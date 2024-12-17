#ifndef GAME_H
#define GAME_H

#include <vector>
#include <functional>
#include <array>
#include <algorithm>
#include <utility>
#include <random>
#include <future>
#include <mutex>
#include <shared_mutex>
#include "utils.hpp"
#include "constant.hpp"

namespace game {
    namespace player {
        class Player {
            private:
            cashType cash;
            int position;
            int prisonTime;
            mutable std::shared_mutex mtx;

            public:
            Player(cashType initialCash = constant::initialCash) : cash(initialCash), position(0), prisonTime(0) {}
            virtual ~Player() {}

            cashType getCash() {
                std::shared_lock<std::shared_mutex> lock(mtx);
                return cash;
            }

            void setCash(cashType newCash) {
                std::unique_lock<std::shared_mutex> lock(mtx);
                cash = newCash;
            }

            cashType addCash(cashType delta) {
                std::unique_lock<std::shared_mutex> lock(mtx);
                cash += delta;
                return cash;
            }

            int getPosition() const { 
                std::shared_lock<std::shared_mutex> lock(mtx);
                return position; 
            }

            void setPosition(int pos) { 
                std::unique_lock<std::shared_mutex> lock(mtx);
                position = pos; 
            }
            
            int getPrisonTime() const { 
                std::shared_lock<std::shared_mutex> lock(mtx);
                return prisonTime;
            }

            void setPrisonTime(int time) { 
                std::unique_lock<std::shared_mutex> lock(mtx);
                prisonTime = time;
            }
        };

        class ComputerPlayer : Player {
            private:
            std::shared_mutex mtxComputer;
            public:
            ComputerPlayer(cashType initialCash = constant::initialCash) : Player(initialCash) {}
            ~ComputerPlayer() override {}
        };
    }

    namespace gamePlay {        
        class Tile {
            public:
            enum TileType {placeholder, home, buildable, prison, tax, random};

            private:
            TileType type;
            mutable std::shared_mutex mtx;

            public:
            Tile(TileType _type = placeholder) : type(_type) {}
            virtual ~Tile() {}

            TileType getType() const { 
                std::shared_lock<std::shared_mutex> lock(mtx);
                return type;
            }
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
                      cashType _basicRent = constant::defaultBasicRent, std::array<game::cashType, 6>& _houseRent = constant::defaultHouseRent) 
                    : Tile(buildable), plotCost(_plotCost), houseCost(_houseCost), basicRent(_basicRent), houseRent(_houseRent), owner(nullptr), status({0, 0}) {}
            ~Buildable() override {}

            bool isOwned() const {
                std::shared_lock<std::shared_mutex> lock(mtxBuildable);
                return owner != nullptr;
            }

            player::Player* getOwner() const {
                std::shared_lock<std::shared_mutex> lock(mtxBuildable);
                return owner;
            }

            void setOwner(player::Player* newOwner) {
                std::unique_lock<std::shared_mutex> lock(mtxBuildable);
                owner = newOwner;
            }

            cashType getPlotCost() const {
                std::shared_lock<std::shared_mutex> lock(mtxBuildable);
                return plotCost;
            }

            cashType getHouseCost() const {
                std::shared_lock<std::shared_mutex> lock(mtxBuildable);
                return houseCost;
            }

            cashType getRent() const {
                std::shared_lock<std::shared_mutex> lock(mtxBuildable);
                // Dynamic rent TODO
                cashType rent = basicRent;
                if (status.house) 
                    rent += houseRent[status.house - 1];
                return rent;
            }

            buildStatus getStatus() const {
                std::shared_lock<std::shared_mutex> lock(mtxBuildable);
                return status;
            }

            void setStatus(buildStatus newStatus) {
                std::unique_lock<std::shared_mutex> lock(mtxBuildable);
                status = newStatus;
            } 
        };

        class Home : public Tile {
            private:
            mutable std::shared_mutex mtxHome;
            public:
            Home() : Tile(home) {}
            ~Home() override {}
        };

        class Prison : public Tile {
            private:
            mutable std::shared_mutex mtxPrison;
            public:
            Prison() : Tile(prison) {}
            ~Prison() override {}
        };

        class Tax : public Tile {
            private:
            double taxRate;
            mutable std::shared_mutex mtxTax;

            public:
            Tax(double _rate = constant::defaultTaxRate) : Tile(tax), taxRate(_rate) {}
            ~Tax() override {}

            double getTaxRate() const {
                std::shared_lock<std::shared_mutex> lock(mtxTax);
                return taxRate;
            }

            void setTaxRate(double newRate) {
                std::unique_lock<std::shared_mutex> lock(mtxTax);
                taxRate = newRate;
            }
        };

        class Random : public Tile {
            private:
            mutable std::shared_mutex mtxRandom;

            public:
            Random() : Tile(random) {};
            ~Random() override {}
        };

        class GameInstance {
            private:
            std::vector<Tile*> tiles;
            std::vector<player::Player*> players;
            int currentPlayerIndex;
            mutable std::shared_mutex mtx;

            GameInstance() : currentPlayerIndex(0) {
                utils::Logger::getInstance();
            }

            ~GameInstance() {}

            GameInstance(const GameInstance&) = delete;

            GameInstance& operator=(const GameInstance&) = delete;

            // Shouldn't be called by the client to avoid mutex deadlock.
            void nextPlayer() {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
                utils::Logger::getInstance().log("nextPlayer(): Next player.");
            }

            // Shouldn't be called by the client to avoid mutex deadlock.
            void movePlayer(player::Player* player, int steps) {
                int origPosition = player->getPosition();
                int overPosition = (origPosition + steps);
                int newPosition = overPosition % tiles.size();
                utils::Logger::getInstance().log("movePlayer(): Player moving from " + std::to_string(origPosition) + " to " + std::to_string(newPosition) + ".");
                player->setPosition(newPosition);
                int i = steps - 1;
                auto it = tiles.begin() + origPosition;
                if (it == tiles.end()) 
                    it = tiles.begin();
                while (i--) {
                    if (++it == tiles.end()) 
                        it = tiles.begin();
                    if ((*it)->getType() == Tile::TileType::home) {
                        player->addCash(constant::homeReward);
                        utils::Logger::getInstance().log("movePlayer(): Rewarding player " + std::to_string(constant::homeReward) + " for passing home.");
                        callbackHomeReward(player, static_cast<Home*>(*it), constant::homeReward);
                    }
                }
                callbackPlayerUpdate(player);
                utils::Logger::getInstance().log("movePlayer(): Handling tile event.");
                handleTileEvent(player, tiles[newPosition]);
            }

            // Shouldn't be called by the client to avoid mutex deadlock.
            void handleTileEvent(player::Player* player, Tile* tile) {
                switch (tile->getType()) {
                    case Tile::buildable: {
                        Buildable* buildableTile = static_cast<Buildable*>(tile);
                        if (!buildableTile->isOwned()) {
                            if (player->getCash() >= buildableTile->getPlotCost() && callbackBuy(player, buildableTile, buildableTile->getPlotCost())) {
                                player->setCash(player->getCash() - buildableTile->getPlotCost());
                                buildableTile->setOwner(player);
                            } else {
                                auto auctionResult = callbackAuction(buildableTile, constant::auctionReservePrice, constant::auctionBidIncrement);
                                if (auctionResult.first >= constant::auctionReservePrice) {
                                    auctionResult.second->addCash(-auctionResult.first);
                                    buildableTile->setOwner(auctionResult.second);
                                }
                            }
                        } else if (buildableTile->getOwner() != player) {
                            player->addCash(-buildableTile->getRent());
                            buildableTile->getOwner()->addCash(buildableTile->getRent());
                            callbackRent(player, buildableTile, buildableTile->getRent());
                        } else if (buildableTile->getOwner() == player) {
                            // Build
                            // TODO
                        }
                        break;
                    }
                    case Tile::prison:
                        player->setPrisonTime(0);
                        callbackPrison(player, static_cast<Prison*>(tile));
                        break;
                    case Tile::tax: {
                        Tax* taxTile = static_cast<Tax*>(tile);
                        cashType taxToPay = static_cast<cashType>(player->getCash() * taxTile->getTaxRate() / 100.0f) * 100;
                        utils::Logger::getInstance().log("handleTileEvent(): Player paid tax " + std::to_string(taxTile->getTaxRate()) + " * " + std::to_string(player->getCash()) + " = " + std::to_string(taxToPay) + ".");
                        player->addCash(-taxToPay);
                        callbackTax(player, taxTile, taxToPay);
                        break;
                    }
                    case Tile::random:
                        // TODO
                        break;
                    case Tile::home:
                        // Maybe some rewards?
                        break;
                    default:
                        break;
                }
            }
            public: 
            // Notifies the client when a player's data has been modified or should be updated, if no other callback functions are triggered.
            std::function<void(player::Player* player)> callbackPlayerUpdate = [](player::Player*)->void{};

            // Player has thrown two dice.
            std::function<void(player::Player* player, int diceValue1, int diceValue2)> callbackDice = [](player::Player*, int, int)->void{};
            
            // Player has thrown a 3rd die.
            std::function<void(player::Player* player, int diceValue3)> callbackDice3rd = [](player::Player*, int)->void{};

            // Player has been jailed.
            std::function<void(player::Player* player, Prison* tile)> callbackPrison = [](player::Player*, Prison*)->void{};

            // Whether the player should pay to escape the prison if they can.
            std::function<bool(player::Player* player, Prison* tile, cashType cashToPay)> callbackPrisonPayOut = [](player::Player*, Prison*, cashType)->bool{ return false; };

            // Whether the player should buy a tile if they can.
            std::function<bool(player::Player* player, Buildable* tile, cashType cashToPay)> callbackBuy = [](player::Player*, Buildable*, cashType)->bool{ return true; };

            // Player has paid rent.
            std::function<void(player::Player* player, Buildable* tile, cashType cashPaid)> callbackRent = [](player::Player*, Buildable*, cashType)->void{};

            // Player has paid tax.
            std::function<void(player::Player* player, Tax* tile, cashType cashPaid)> callbackTax = [](player::Player*, Tax*, cashType)->void{};

            // Player has received cash for passing startpoint.
            std::function<void(player::Player* player, Home* tile, cashType cashReceived)> callbackHomeReward = [](player::Player*, Home*, cashType)->void{};

            // A tile should be auctioned. Returning value less than reserve price or nullptr means no one has participated or the auction has failed.
            std::function<std::pair<cashType, player::Player*>(Buildable* tile, cashType reservePrice, cashType bidIncrement)> callbackAuction 
                = [](Buildable*, cashType, cashType)->std::pair<cashType, player::Player*>{ return std::make_pair<cashType, player::Player*>(0, nullptr); };
            
            // Player has to sell their own properties if the total value of their properties can cover the debt. WIP
            //std::function<std::vector<Tile*>(player::Player* player, std::vector<Tile*>& tilesToBeSold)> callbackAuction;
            
            // Whether the player should build a house or hotel if they can. WIP
            //std::function<bool(player::Player* player, Buildable* tile)> callbackHouseBuild = [](player::Player*, Buildable*)->bool{ return true; };
            
            // Notifies the client when a tile's data has been modified, if no other callback functions are triggered.
            std::function<void(Tile* tile)> callbackTileUpdate = [](Tile*)->void{};

            static GameInstance& getInstance() {
                static GameInstance instance;
                return instance;
            }

            const std::vector<Tile*>& getTiles() const {
                std::shared_lock<std::shared_mutex> lock(mtx);
                return tiles;
            }

            const std::vector<player::Player*>& getPlayers() const {
                std::shared_lock<std::shared_mutex> lock(mtx);
                return players;
            }
            
            const int findPlayerPos(player::Player* p) const {
                std::shared_lock<std::shared_mutex> lock(mtx);
                auto it = std::find(players.begin(), players.end(), p);
                if (it != players.end()) 
                    return it - players.begin();
                return -1;
            }

            const std::vector<Tile*> findOwnTiles(player::Player* p) const {
                std::shared_lock<std::shared_mutex> lock(mtx);
                std::vector<Tile*> ownTiles;
                std::copy_if(tiles.begin(), tiles.end(), std::back_inserter(ownTiles), [p](Tile* t) {
                    if (t->getType() == Tile::buildable) {
                        return static_cast<Buildable*>(t)->getOwner() == p;
                    }
                    return false;
                });
                return ownTiles;
            }

            const int findNextTile(Tile::TileType type, int pos) const {
                std::shared_lock<std::shared_mutex> lock(mtx);
                auto it = tiles.begin() + pos;
                for (int i = 0; i < tiles.size(); i++) {
                    if (++it == tiles.end()) 
                        it = tiles.begin();
                }
                return -1;
            }

            const int findTile(Tile* tile) const {
                std::shared_lock<std::shared_mutex> lock(mtx);
                auto it = std::find(tiles.begin(), tiles.end(), tile);
                if (it != tiles.end()) 
                    return it - tiles.begin();
                return -1;
            }

            void addTile(Tile* tile) {
                std::unique_lock<std::shared_mutex> lock(mtx);
                tiles.push_back(tile);
            }

            void addPlayer(player::Player* player) {
                std::unique_lock<std::shared_mutex> lock(mtx);
                players.push_back(player);
            }

            static int rollDice(int minimum = constant::diceMinimum, int maximum = constant::diceMaximum) {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<int> dis(minimum, maximum);
                return dis(gen);
            }

            player::Player* getCurrentPlayer() const {
                std::shared_lock<std::shared_mutex> lock(mtx);
                return players[currentPlayerIndex];
            }

            void tick() {
                utils::Logger::getInstance().log("tick(): Trying to acquire lock...");

                // Use local variables to avoid deadlock
                player::Player* currentPlayer;
                int currentPosition;
                Tile::TileType currentTileType;

                {
                    std::shared_lock<std::shared_mutex> lock(mtx);
                    currentPlayer = getCurrentPlayer();
                    currentPosition = currentPlayer->getPosition();
                    currentTileType = tiles[currentPosition]->getType();
                }

                utils::Logger::getInstance().log("tick(): Lock released.");
                utils::Logger::getInstance().log("tick(): Tick start. Player: " + std::to_string(currentPlayerIndex) + ", Position: " + std::to_string(currentPosition) + ".");

                // Check if in prison
                if (currentTileType == Tile::TileType::prison) {
                    utils::Logger::getInstance().log("tick(): Player is in prison.");

                    // If player has been in prison for less than 3 turns
                    if (currentPlayer->getPrisonTime() < 3) {
                        for (int i = 0; i < 3; i++) {
                            utils::Logger::getInstance().log("tick(): Rolling dice for prison, try: " + std::to_string(i) + ".");
                            int diceValue1 = rollDice(), diceValue2 = rollDice();
                            callbackDice(currentPlayer, diceValue1, diceValue2);
                            if (diceValue1 == diceValue2) {
                                utils::Logger::getInstance().log("tick(): Player successfully got out of prison.");
                                movePlayer(currentPlayer, diceValue1 + diceValue2);
                                nextPlayer();
                                return;
                            }
                        }

                        // Pay to free
                        if (currentPlayer->getCash() >= constant::prisonReleasePrice[currentPlayer->getPrisonTime()] 
                            && callbackPrisonPayOut(currentPlayer, static_cast<Prison*>(tiles[currentPosition]), constant::prisonReleasePrice[currentPlayer->getPrisonTime()])) {
                            utils::Logger::getInstance().log("tick(): Player paid to get out of prison.");
                            currentPlayer->addCash(-constant::prisonReleasePrice[currentPlayer->getPrisonTime()]);
                            currentPlayer->setPrisonTime(0);
                        } else {
                            utils::Logger::getInstance().log("tick(): Player failed to get out of prison.");
                            currentPlayer->setPrisonTime(currentPlayer->getPrisonTime() + 1);
                            nextPlayer();
                            return;
                        }
                    }
                }

                // Roll dice
                int diceValue1 = rollDice(), diceValue2 = rollDice();
                utils::Logger::getInstance().log("tick(): Player rolled dice " + std::to_string(diceValue1) + ", " + std::to_string(diceValue2) + ".");
                callbackDice(currentPlayer, diceValue1, diceValue2);

                if (diceValue1 == diceValue2) {
                    int diceValue3 = rollDice();
                    utils::Logger::getInstance().log("tick(): Player rolling a third dice " + std::to_string(diceValue3) + ".");
                    callbackDice3rd(currentPlayer, diceValue3);

                    if (diceValue3 == diceValue1) {
                        int prisonPos;
                        {
                            // Lock already added in the util function
                            prisonPos = findNextTile(Tile::TileType::prison, currentPosition);
                        }

                        if (prisonPos != -1) {
                            {
                                currentPlayer->setPosition(prisonPos);
                                currentPlayer->setPrisonTime(0);
                            }
                            utils::Logger::getInstance().log("tick(): Player prisoned for 3 same dice.");
                            callbackPrison(currentPlayer, static_cast<Prison*>(tiles[prisonPos]));
                        } else {
                            utils::Logger::getInstance().log("tick(): No prison found.");
                        }
                    } else {
                        movePlayer(currentPlayer, diceValue1 + diceValue2 + diceValue3);
                    }
                } else {
                    movePlayer(currentPlayer, diceValue1 + diceValue2);
                }

                nextPlayer();
            }

        };
    }
}

#endif