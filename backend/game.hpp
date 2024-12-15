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
                      cashType _basicRent = constant::defaultBasicRent, auto& _houseRent = constant::defaultHouseRent) 
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

            // Player has thrown a dice.
            std::function<void(player::Player* player, int diceValue)> callbackDice = [](player::Player*, int)->void{};

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
                // TODO: CHECK IF IN PRISON
                int diceValue = rollDice();
                callbackDice(getCurrentPlayer(), diceValue);
                movePlayer(getCurrentPlayer(), diceValue);
                nextPlayer();
            }

            void nextPlayer() {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            }

            void movePlayer(player::Player* player, int steps) {
                int origPosition = player->getPosition();
                int overPosition = (origPosition + steps);
                int newPosition = overPosition % tiles.size();
                player->setPosition(newPosition);
                int i = steps;
                auto it = tiles.begin() + origPosition;
                while (i--) {
                    if (it == tiles.end()) 
                        it = tiles.begin();
                    else 
                        it++;
                    if ((*it)->getType() == Tile::TileType::home) {
                        player->addCash(constant::homeReward);
                        callbackHomeReward(player, static_cast<Home*>(*it), constant::homeReward);
                    }
                }
                callbackPlayerUpdate(player);
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
    
    namespace player {
        class Player {
            private:
            cashType cash;
            int position;

            public:
            Player(cashType initialCash = constant::initialCash) : cash(initialCash), position(0) {}
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
        };

        class ComputerPlayer : Player {
            private:
            public:
            ComputerPlayer(cashType initialCash = constant::initialCash) : Player(initialCash) {}
            ~ComputerPlayer() override {}
        };
    }
}

#endif