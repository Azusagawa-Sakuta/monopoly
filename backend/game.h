#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <array>
#include <any>
#include <QObject>
#include "constant.h"

namespace game {
    namespace player {
        class Player {
        private:
            cashType cash;
            int position;
            int prisonTime;
            std::string nickname;
            std::string imagePath;
            bool bankrupted;

        public:
            Player(cashType initialCash = constant::initialCash);
            virtual ~Player();

            cashType getCash();
            void setCash(cashType newCash);
            cashType addCash(cashType delta);
            int getPosition() const;
            void setPosition(int pos);
            int getPrisonTime() const;
            void setPrisonTime(int time);
            std::string getNickname() const;
            void setNickname(const std::string& newNickname);
            std::string getImagePath() const;
            void setImagePath(const std::string& newImagePath);
            bool isBankrupted() const;
            void setBankrupted(bool newBankrupted);
            virtual bool isComputer() const;
        };

        class ComputerPlayer : public Player {
        public:
            ComputerPlayer(cashType initialCash = constant::initialCash);
            ~ComputerPlayer() override;
            bool isComputer() const override;
        };
    }

    namespace gamePlay {
        class Tile {
        public:
            enum TileType { placeholder, home, buildable, prison, tax, random };

        private:
            TileType type;

        public:
            Tile(TileType _type = placeholder);
            virtual ~Tile();

            TileType getType() const;
        };

        class Buildable : public Tile {
        public:
            enum buildStatus { empty, house1, house2, house3, house4, house5, hotel };

        private:
            cashType plotCost;
            cashType houseCost;
            cashType basicRent;
            std::array<cashType, 6> houseRent;
            player::Player* owner;
            buildStatus status;
            int color;

        public:
            Buildable(cashType _plotCost = constant::defaultPlotCost, cashType _houseCost = constant::defaultHouseCost,
                      cashType _basicRent = constant::defaultBasicRent, std::array<game::cashType, 6>& _houseRent = constant::defaultHouseRent);
            ~Buildable() override;

            bool isOwned() const;
            player::Player* getOwner() const;
            void setOwner(player::Player* newOwner);
            cashType getPlotCost() const;
            cashType getHouseCost() const;
            cashType getRent() const;
            buildStatus getStatus() const;
            void setStatus(buildStatus newStatus);
            int getColor() const;
            void setColor(int newColor);
            cashType getValue() const;
        };

        class Home : public Tile {
        public:
            Home();
            ~Home() override;
        };

        class Prison : public Tile {
        public:
            Prison();
            ~Prison() override;
        };

        class Tax : public Tile {
        private:
            double taxRate;

        public:
            Tax(double _rate = constant::defaultTaxRate);
            ~Tax() override;

            double getTaxRate() const;
            void setTaxRate(double newRate);
        };

        class Random : public Tile {
        public:
            Random();
            ~Random() override;
        };

        // State machine steps for the game loop
        enum class GameStep {
            Idle,
            RollingDice,
            AnimatingDice,
            MovingPlayer,
            HandlingTile,
            WaitingBuyDecision,
            WaitingPrisonDice,
            WaitingPrisonPayOut,
            WaitingAuction,
            WaitingBuild,
            WaitingSell,
            WaitingUpdate,
            WaitingHomeReward,
            TurnEnd,
            GameOver
        };

        class GameInstance : public QObject {
            Q_OBJECT

        public:
            struct auctionResult {
                cashType price;
                player::Player* player;
            };

            struct auctionRequest {
                cashType reservePrice;
                cashType bidIncrement;
                Buildable* tile;
            };

            struct buyRequest {
                cashType price;
                Buildable* tile;
            };

            struct rentRequest {
                cashType rent;
                Buildable* tile;
            };

            struct taxRequest {
                cashType tax;
                Tax* tile;
            };

        private:
            std::vector<Tile*> tiles;
            std::vector<player::Player*> players;
            int currentPlayerIndex;
            GameStep currentStep;

            // Saved context for resuming after user input
            player::Player* savedPlayer;
            Tile* savedTile;
            cashType savedAmount;
            GameStep returnStep;

            GameInstance();
            ~GameInstance();
            GameInstance(const GameInstance&) = delete;
            GameInstance& operator=(const GameInstance&) = delete;

            void nextPlayer();
            void movePlayer(player::Player* player, int steps);
            void handleTileEvent(player::Player* player, Tile* tile);

        public:
            static GameInstance& getInstance();
            const std::vector<Tile*>& getTiles() const;
            const std::vector<player::Player*>& getPlayers() const;
            int findPlayerIndex(player::Player* p) const;
            const std::vector<Tile*> findOwnTiles(player::Player* p) const;
            int findNextTile(Tile::TileType type, int pos) const;
            int findTile(Tile* tile) const;
            void addTile(Tile* tile);
            void addPlayer(player::Player* player);
            player::Player* getCurrentPlayer() const;

            // New event-driven API
            void advance();
            void provideInput(const std::any& result);

        signals:
            // State transitions
            void turnStarted(game::player::Player* player);
            void gameEnded(game::player::Player* winner);

            // Dice and movement
            void diceRolled(game::player::Player* player, int dice1, int dice2);
            void playerMoved(game::player::Player* player, int fromPosition, int toPosition);
            void playerPassedGo(game::player::Player* player, cashType reward);

            // Tile events (automatic — just notify UI)
            void tileLanded(game::player::Player* player, game::gamePlay::Tile* tile);
            void rentPaid(game::player::Player* payer, game::player::Player* owner, game::gamePlay::Buildable* tile, game::cashType amount);
            void taxPaid(game::player::Player* player, game::gamePlay::Tax* tile, game::cashType amount);
            void homeRewardCollected(game::player::Player* player, game::cashType amount);
            void randomEventTriggered(game::gamePlay::Tile* tile, const QString& description);
            void playerPrisoned(game::player::Player* player, game::gamePlay::Prison* tile);

            // Tile events (need human input — UI must call provideInput)
            void buyDecisionNeeded(game::player::Player* player, game::gamePlay::Buildable* tile, game::cashType cost);
            void prisonDiceNeeded(game::player::Player* player);
            void prisonPayOutNeeded(game::player::Player* player, game::cashType cost);
            void auctionNeeded(game::gamePlay::Buildable* tile, game::cashType reservePrice, game::cashType bidIncrement);
            void buildDecisionNeeded(game::player::Player* player, game::gamePlay::Buildable* tile, game::cashType houseCost, int maxLevels);
            void sellDecisionNeeded(game::player::Player* player, game::cashType amountNeeded);

            // State changes
            void playerUpdated(game::player::Player* player);
            void playerBankrupted(game::player::Player* player);
            void propertyChanged(game::gamePlay::Buildable* tile);
            void boardUpdateNeeded();
        };
    }
}

#endif
