#ifndef GAME_H
#define GAME_H

#include <vector>
#include <functional>
#include <array>
#include <shared_mutex>
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
            struct buildStatus {
                int house;
                int hotel;
            };

        private:
            cashType plotCost;
            cashType houseCost;
            cashType basicRent;
            std::array<cashType, 6> houseRent;
            player::Player* owner;
            buildStatus status;
            mutable std::shared_mutex mtxBuildable;

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
        private:
            std::vector<Tile*> tiles;
            std::vector<player::Player*> players;
            int currentPlayerIndex;
            mutable std::shared_mutex mtx;

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
            std::function<bool(player::Player* player, Prison* tile, cashType cashToPay)> callbackPrisonPayOut;
            std::function<bool(player::Player* player, Buildable* tile, cashType cashToPay)> callbackBuy;
            std::function<void(player::Player* player, Buildable* tile, cashType cashPaid)> callbackRent;
            std::function<void(player::Player* player, Tax* tile, cashType cashPaid)> callbackTax;
            std::function<void(player::Player* player, Home* tile, cashType cashReceived)> callbackHomeReward;
            std::function<std::pair<cashType, player::Player*>(Buildable* tile, cashType reservePrice, cashType bidIncrement)> callbackAuction;
            std::function<void(Tile* tile)> callbackTileUpdate;

            static GameInstance& getInstance();
            const std::vector<Tile*>& getTiles() const;
            const std::vector<player::Player*>& getPlayers() const;
            const int findPlayerPos(player::Player* p) const;
            const std::vector<Tile*> findOwnTiles(player::Player* p) const;
            const int findNextTile(Tile::TileType type, int pos) const;
            const int findTile(Tile* tile) const;
            void addTile(Tile* tile);
            void addPlayer(player::Player* player);
            static int rollDice(int minimum = constant::diceMinimum, int maximum = constant::diceMaximum);
            player::Player* getCurrentPlayer() const;
            void tick();
        };
    }
}

#endif