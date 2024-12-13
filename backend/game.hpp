#ifndef GAME_H
#define GAME_H

#include <vector>
#include <functional>
#include <array>
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
            cashType hotelCost;
            cashType basicRent;
            std::array<cashType, 5> houseRent;
            cashType hotelRent;
            bool owned;
            player::Player* owner;
            buildStatus status;

            public:
            Buildable(cashType _plotCost = constant::defaultPlotCost, cashType _houseCost = constant::defaultHouseCost, cashType _hotelCost = constant::defaultHotelCost, 
                      cashType _basicRent = constant::defaultBasicRent, auto& _houseRent = constant::defaultHouseRent, cashType _hotelRent = constant::defaultHotelRent) 
                    : Tile(buildable), plotCost(_plotCost), houseCost(_houseCost), hotelCost(_hotelCost), basicRent(_basicRent), houseRent(_houseRent), hotelRent(_hotelRent), 
                      owned(false), owner(nullptr), status({0, 0}) {}
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
            
            cashType getHotelCost() const {
                return hotelCost;
            }

            cashType getRent() const {
                // Dynamic rent TODO
                cashType rent = basicRent;
                if (status.house) 
                    rent += houseRent[status.house - 1];
                if (status.hotel) 
                    rent += hotelRent;
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
                            if (player->getCash() >= buildableTile->getPlotCost()) {
                                if (callbackBuy(player, buildableTile)) {
                                    player->setCash(player->getCash() - buildableTile->getPlotCost());
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