#ifndef GAME_H
#define GAME_H

#include <vector>
#include <functional>

namespace game {
    typedef long long cashType;

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
            Buildable(cashType _cost = 0, cashType _rent = 0) : Tile(buildable), cost(_cost), rent(_rent), owned(false), owner(nullptr) {}
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
            Tax(double _rate = 0.0f) : Tile(tax), taxRate(_rate) {}
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

            public:
            std::function<bool(player::Player* player, Buildable* tile)> callbackBuild = [](player::Player*, Buildable*)->bool{ return true; };
            std::function<void(player::Player* player, Buildable* tile)> callbackRent = [](player::Player*, Buildable*)->void{};
            std::function<void(player::Player* player, Buildable* tile)> callbackTax = [](player::Player*, Buildable*)->void{};

            GameInstance() : currentPlayerIndex(0) {}
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
                            // UI Callback event to be added
                            if (callbackBuild(player, buildableTile)) {
                                if (player->getCash() >= buildableTile->getCost()) {
                                    player->setCash(player->getCash() - buildableTile->getCost());
                                    buildableTile->setOwner(player);
                                }
                            }
                        } else if (buildableTile->getOwner() != player) {
                            // Pay rent
                            player->setCash(player->getCash() - buildableTile->getRent());
                            buildableTile->getOwner()->setCash(buildableTile->getOwner()->getCash() + buildableTile->getRent());
                            callbackRent(player, buildableTile);
                        }
                        break;
                    }
                    case Tile::prison:
                        break;
                    case Tile::tax: {
                        Tax* taxTile = static_cast<Tax*>(tile);
                        //player->setCash(player->getCash() - taxTile->getTaxAmount());
                        break;
                    }
                    case Tile::random:
                        break;
                    case Tile::home:
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

            Player(cashType initialCash = 0) : cash(initialCash), position(0) {}
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

            ComputerPlayer(cashType initialCash = 0) : Player(initialCash) {}
            ~ComputerPlayer() override {}
        };
    }
}

#endif