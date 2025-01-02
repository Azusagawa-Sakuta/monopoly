#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <functional>
#include <array>
#include <shared_mutex>
#include <any>
#include <condition_variable>
#include <variant>
#include "utils.h"
#include "constant.h"

namespace game {
    namespace player {
        class Player {
        private:
            cashType cash;
            int position;
            int prisonTime;
            mutable std::shared_mutex mtx;
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
        };

        class ComputerPlayer : public Player {
        private:
            std::shared_mutex mtxComputer;

        public:
            ComputerPlayer(cashType initialCash = constant::initialCash);
            ~ComputerPlayer() override;
        };
    }

    namespace gamePlay {
        class Tile {
        public:
            enum TileType { placeholder, home, buildable, prison, tax, random };

        private:
            TileType type;
            mutable std::shared_mutex mtx;

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
            mutable std::shared_mutex mtxBuildable;
            mutable std::condition_variable_any cv;
            mutable bool userInputReady = false;
            mutable std::any userInputResult;

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
        private:
            mutable std::shared_mutex mtxHome;

        public:
            Home();
            ~Home() override;
        };

        class Prison : public Tile {
        private:
            mutable std::shared_mutex mtxPrison;

        public:
            Prison();
            ~Prison() override;
        };

        class Tax : public Tile {
        private:
            double taxRate;
            mutable std::shared_mutex mtxTax;

        public:
            Tax(double _rate = constant::defaultTaxRate);
            ~Tax() override;

            double getTaxRate() const;
            void setTaxRate(double newRate);
        };

        class Random : public Tile {
        private:
            mutable std::shared_mutex mtxRandom;

        public:
            Random();
            ~Random() override;
        };

        class GameInstance {
        public:
            enum eventType {
                None, Update, Dice, Prisoned, PrisonPayOut, PrisonDice, Buy, RentPaid, Taxed, HomeReward, Auction, Build, RandomDestruction, RandomEarn, Sell
            };

        private:
            std::vector<Tile*> tiles;
            std::vector<player::Player*> players;
            int currentPlayerIndex;
            mutable std::shared_mutex mtx;
            mutable std::condition_variable_any cv;
            mutable bool userInputReady = false;
            mutable std::any userInputResult;
            mutable eventType activeEvent;
            mutable std::any eventParam;

            GameInstance();
            ~GameInstance();
            GameInstance(const GameInstance&) = delete;
            GameInstance& operator=(const GameInstance&) = delete;

            void nextPlayer();
            void movePlayer(player::Player* player, int steps);
            void handleTileEvent(player::Player* player, Tile* tile);

        public:
            std::function<void(player::Player* player)> callbackPlayerUpdate;
            std::function<void(player::Player* player, int diceValue1, int diceValue2)> callbackDice;
            std::function<void(player::Player* player, int diceValue3)> callbackDice3rd;
            std::function<void(player::Player* player, Prison* tile)> callbackPrison;
            std::function<void(player::Player* player, Prison* tile, cashType cashToPay)> callbackPrisonPayOut;
            std::function<void(player::Player* player, Buildable* tile, cashType cashToPay)> callbackBuy;
            std::function<void(player::Player* player, Buildable* tile, cashType cashPaid)> callbackRent;
            std::function<void(player::Player* player, Tax* tile, cashType cashPaid)> callbackTax;
            std::function<void(player::Player* player, Home* tile, cashType cashReceived)> callbackHomeReward;
            std::function<void(Buildable* tile, cashType reservePrice, cashType bidIncrement)> callbackAuction;
            std::function<void(Tile* tile)> callbackTileUpdate;

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

            eventType getActiveEvent() const;
            std::any getActiveEventParam() const;
            void endEvent();
            void notifyUserInput(const std::any result);
            std::any waitForUserInput(eventType event, std::any param = std::monostate());
            static GameInstance& getInstance();
            const std::vector<Tile*>& getTiles() const;
            const std::vector<player::Player*>& getPlayers() const;
            const int findPlayerPos(player::Player* p) const;
            const std::vector<Tile*> findOwnTiles(player::Player* p) const;
            const int findNextTile(Tile::TileType type, int pos) const;
            const int findTile(Tile* tile) const;
            void addTile(Tile* tile);
            void addPlayer(player::Player* player);
            player::Player* getCurrentPlayer() const;
            void tick();
        };
    }
}

#endif
