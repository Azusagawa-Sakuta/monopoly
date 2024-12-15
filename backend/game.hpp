#ifndef GAME_H
#define GAME_H

#include <vector>
#include <functional>
#include <array>
#include <algorithm>
#include <utility>
#include <random>
#include "utils.hpp"
#include "constant.hpp"

namespace game {
    namespace player {
        class Player {
            private:
            cashType cash;
            int position;
            int prisonTime;

            public:
            Player(cashType initialCash = constant::initialCash) : cash(initialCash), position(0), prisonTime(0) {}
            virtual ~Player() {}

            cashType getCash() {
                return cash;
            }

            void setCash(cashType newCash) {
                cash = newCash;
            }

            cashType addCash(cashType delta) {
                cash += delta;
                return cash;
            }

            int getPosition() const { 
                return position; 
            }

            void setPosition(int pos) { 
                position = pos; 
            }
            
            int getPrisonTime() const {
                return prisonTime;
            }

            void setPrisonTime(int time) {
                prisonTime = time;
            }
        };

        class ComputerPlayer : Player {
            private:
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
            // Tile type
            // What should I write here?
            TileType type;

            public:

            Tile(TileType _type = placeholder) : type(_type) {}
            virtual ~Tile() {}

            TileType getType() const {
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
            bool owned;
            player::Player* owner;
            buildStatus status;

            public:
            Buildable(cashType _plotCost = constant::defaultPlotCost, cashType _houseCost = constant::defaultHouseCost, 
                      cashType _basicRent = constant::defaultBasicRent, std::array<game::cashType, 6>& _houseRent = constant::defaultHouseRent) 
                    : Tile(buildable), plotCost(_plotCost), houseCost(_houseCost), basicRent(_basicRent), houseRent(_houseRent), owned(false), owner(nullptr), status({0, 0}) {}
            ~Buildable() override {}

            bool isOwned() const {
                return owned;
            }

            player::Player* getOwner() const {
                return owner;
            }

            void setOwner(player::Player* newOwner) {
                owner = newOwner;
            }

            cashType getPlotCost() const {
                return plotCost;
            }

            cashType getHouseCost() const {
                return houseCost;
            }

            cashType getRent() const {
                // Dynamic rent TODO
                cashType rent = basicRent;
                if (status.house) 
                    rent += houseRent[status.house - 1];
                return rent;
            }

            buildStatus getStatus() const {
                return status;
            }

            void setStatus(buildStatus newStatus) {
                status = newStatus;
            } 
        };

        class Home : public Tile {
            private:
            public:
            Home() : Tile(home) {}
            ~Home() override {}
        };

        class Prison : public Tile {
            private:
            public:
            Prison() : Tile(prison) {}
            ~Prison() override {}
        };

        class Tax : public Tile {
            private:
            double taxRate;

            public:
            Tax(double _rate = constant::defaultTaxRate) : Tile(tax), taxRate(_rate) {}
            ~Tax() override {}

            double getTaxRate() const {
                return taxRate;
            }

            void setTaxRate(double newRate) {
                taxRate = newRate;
            }
        };

        class Random : public Tile {
            private:
            public:
            Random() : Tile(random) {};
            ~Random() override {}
        };

        class GameInstance {
            private:
            std::vector<Tile*> tiles;
            std::vector<player::Player*> players;
            int currentPlayerIndex;

            GameInstance() : currentPlayerIndex(0) {
                utils::Logger::getInstance();
            }

            ~GameInstance() {}

            GameInstance(const GameInstance&) = delete;

            GameInstance& operator=(const GameInstance&) = delete;

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
                return tiles;
            }

            const std::vector<player::Player*>& getPlayers() const {
                return players;
            }
            
            const std::vector<Tile*> findOwnTiles(player::Player* p) const {
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
                auto it = tiles.begin() + pos;
                for (int i = 0; i < tiles.size(); i++) {
                    if (++it == tiles.end()) 
                        it = tiles.begin();
                }
                return -1;
            }

            const int findTile(Tile* tile) const {
                auto it = std::find(tiles.begin(), tiles.end(), tile);
                if (it != tiles.end()) 
                    return it - tiles.begin();
                return -1;
            }

            void addTile(Tile* tile) {
                tiles.push_back(tile);
            }

            void addPlayer(player::Player* player) {
                players.push_back(player);
            }

            static int rollDice(int minimum = constant::diceMinimum, int maximum = constant::diceMaximum) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> dis(minimum, maximum);
                return dis(gen);
            }

            player::Player* getCurrentPlayer() const {
                return players[currentPlayerIndex];
            }

            void tick() {
                utils::Logger::getInstance().log("Tick start. Player: " + std::to_string(currentPlayerIndex) + ", Position: " + std::to_string(getCurrentPlayer()->getPosition()) + ".");
                if (tiles[getCurrentPlayer()->getPosition()]->getType() == Tile::TileType::prison) {
                    utils::Logger::getInstance().log("Player is in prison.");
                    // What the fricking hell is this?
                    if (getCurrentPlayer()->getPrisonTime() < 3) { 
                        for (int i = 0; i < 3; i++) {
                            int diceValue1 = rollDice(), diceValue2 = rollDice();
                            callbackDice(getCurrentPlayer(), diceValue1, diceValue2);
                            if (diceValue1 == diceValue2) {
                                movePlayer(getCurrentPlayer(), diceValue1 + diceValue2);
                                nextPlayer();
                                return;
                            }
                        }
                        if (getCurrentPlayer()->getCash() >= constant::prisonReleasePrice[getCurrentPlayer()->getPrisonTime()] 
                            && callbackPrisonPayOut(getCurrentPlayer(), static_cast<Prison*>(tiles[getCurrentPlayer()->getPosition()]), constant::prisonReleasePrice[getCurrentPlayer()->getPrisonTime()])) {
                            getCurrentPlayer()->addCash(-constant::prisonReleasePrice[getCurrentPlayer()->getPrisonTime()]);
                            getCurrentPlayer()->setPrisonTime(0);
                        } else {
                            getCurrentPlayer()->setPrisonTime(getCurrentPlayer()->getPrisonTime() + 1);
                            nextPlayer();
                            return;
                        }
                    }
                }
                int diceValue1 = rollDice(), diceValue2 = rollDice();
                utils::Logger::getInstance().log("Player rolled dice " + std::to_string(diceValue1) + ", " + std::to_string(diceValue2) + ".");
                callbackDice(getCurrentPlayer(), diceValue1, diceValue2);
                if (diceValue1 == diceValue2) {
                    int diceValue3 = rollDice();
                    utils::Logger::getInstance().log("Player rolling a third dice " + std::to_string(diceValue3) + ".");
                    callbackDice3rd(getCurrentPlayer(), diceValue3);
                    if (diceValue3 == diceValue1) {
                        int prisonPos = findNextTile(Tile::TileType::prison, getCurrentPlayer()->getPosition());
                        if (prisonPos != -1) {
                            getCurrentPlayer()->setPosition(prisonPos);
                            getCurrentPlayer()->setPrisonTime(0);
                            utils::Logger::getInstance().log("Player prisoned for 3 same dice.");
                            callbackPrison(getCurrentPlayer(), static_cast<Prison*>(tiles[prisonPos]));
                        } else 
                            utils::Logger::getInstance().log("No prison found.");
                    } else 
                        movePlayer(getCurrentPlayer(), diceValue1 + diceValue2 + diceValue3);
                } else 
                    movePlayer(getCurrentPlayer(), diceValue1 + diceValue2);
                nextPlayer();
            }

            void nextPlayer() {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
                utils::Logger::getInstance().log("Next player.");
            }

            void movePlayer(player::Player* player, int steps) {
                int origPosition = player->getPosition();
                int overPosition = (origPosition + steps);
                int newPosition = overPosition % tiles.size();
                utils::Logger::getInstance().log("Player moving from " + std::to_string(origPosition) + " to " + std::to_string(newPosition) + ".");
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
                        utils::Logger::getInstance().log("Rewarding player " + std::to_string(constant::homeReward) + " for passing home.");
                        callbackHomeReward(player, static_cast<Home*>(*it), constant::homeReward);
                    }
                }
                callbackPlayerUpdate(player);
                utils::Logger::getInstance().log("Handling tile event.");
                handleTileEvent(player, tiles[newPosition]);
            }

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
                        player->setCash(player->getCash() * (1.0f - taxTile->getTaxRate()));
                        callbackTax(player, taxTile, player->getCash() * taxTile->getTaxRate());
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
        };
    }
}

#endif