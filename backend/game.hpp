#ifndef GAME_H
#define GAME_H

#include <vector>
#include <functional>
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
            private:
            cashType cost;
            cashType rent;
            bool owned;
            player::Player* owner;

            public:
            Buildable(cashType _cost = constant::defaultCost, cashType _rent = constant::defaultRent) : Tile(buildable), cost(_cost), rent(_rent), owned(false), owner(nullptr) {}
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

            int getCost() const {
                return cost;
            }

            int getRent() const {
                // Dynamic rent TODO
                return rent;
            }

            void setCost(int newCost) {
                cost = newCost;
            }

            void setRent(int newRent) {
                rent = newRent;
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
            utils::Logger logger;

            std::vector<Tile*> tiles;
            std::vector<player::Player*> players;
            int currentPlayerIndex;

            public: 
            // Notifies the client when a player's data has been modified, if no other callback functions are triggered.
            std::function<void(player::Player* player)> callbackPlayerUpdate = [](player::Player*)->void{};

            // Whether the player should buy a tile if they can.
            std::function<bool(player::Player* player, Buildable* tile)> callbackBuy = [](player::Player*, Buildable*)->bool{ return true; };

            // Player has paid rent.
            std::function<void(player::Player* player, Buildable* tile)> callbackRent = [](player::Player*, Buildable*)->void{};

            // Player has paid tax.
            std::function<void(player::Player* player, Tax* tile)> callbackTax = [](player::Player*, Tax*)->void{};

            // Player has received cash for passing startpoint.
            std::function<void(player::Player* player, Home* home)> callbackHomeReward = [](player::Player*, Home*)->void{};

            GameInstance() : currentPlayerIndex(0), logger() {}
            ~GameInstance() {}
            
            const std::vector<Tile*>& getTiles() const {
                return tiles;
            }

            const std::vector<player::Player*>& getPlayers() const {
                return players;
            }
            
            void addTile(Tile* tile) {
                tiles.push_back(tile);
            }

            void addPlayer(player::Player* player) {
                players.push_back(player);
            }

            player::Player* getCurrentPlayer() const {
                return players[currentPlayerIndex];
            }

            void nextPlayer() {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            }

            void movePlayer(player::Player* player, int steps) {
                int newPosition = (player->getPosition() + steps) % tiles.size();
                player->setPosition(newPosition);
                handleTileEvent(player, tiles[newPosition]);
            }

            void handleTileEvent(player::Player* player, Tile* tile) {
                switch (tile->getType()) {
                    case Tile::buildable: {
                        Buildable* buildableTile = static_cast<Buildable*>(tile);
                        if (!buildableTile->isOwned()) {
                            if (player->getCash() >= buildableTile->getCost()) {
                                if (callbackBuy(player, buildableTile)) {
                                    player->setCash(player->getCash() - buildableTile->getCost());
                                    buildableTile->setOwner(player);
                                }
                            }
                            // Auction
                            // TODO
                        } else if (buildableTile->getOwner() != player) {
                            // Pay rent
                            player->addCash(-buildableTile->getRent());
                            buildableTile->getOwner()->addCash(buildableTile->getRent());
                            callbackRent(player, buildableTile);
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
                        callbackTax(player, taxTile);
                        break;
                    }
                    case Tile::random:
                        // TODO
                        break;
                    case Tile::home:
                        Home* homeTile = static_cast<Home*>(tile);
                        player->addCash(constant::homeReward);
                        callbackHomeReward(player, homeTile);
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