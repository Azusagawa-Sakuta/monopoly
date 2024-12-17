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
            // Constructor; initializes the player with the given cash amount.
            Player(cashType initialCash = constant::initialCash) : cash(initialCash), position(0), prisonTime(0) {}
            // Destructor; virtual to allow derived classes to override.
            virtual ~Player() {}

            cashType getCash() {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     cashType cash amount of the player
                    * Get the cash amount of the player
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                return cash;                                   // Return the cash amount
            }

            void setCash(cashType newCash) {
                /*
                    * Args:
                    *     cashType: to set new cash amount
                    * Returns:
                    *     None
                    * Set the cash amount of the player
                 */

                std::unique_lock<std::shared_mutex> lock(mtx); // Lock the mutex in unique mode
                cash = newCash;                                // Set the cash amount
            }

            cashType addCash(cashType delta) {
                /*
                    * Args:
                    *     cashType: to add to the cash amount
                    * Returns:
                    *     cashType: cash amount of the player
                    * Add the given amount to the cash amount of the player
                 */

                std::unique_lock<std::shared_mutex> lock(mtx); // Lock the mutex in unique mode
                cash += delta;                                 // Add the given amount to the cash amount
                return cash;                                   // Return the cash amount
            }

            int getPosition() const { 
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     int: position of the player
                    * Get the position of the player
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                return position;                               // Return the position
            }

            void setPosition(int pos) { 
                /*
                    * Args:
                    *     int: position to set
                    * Returns:
                    *     None
                    * Set the position of the player
                 */

                std::unique_lock<std::shared_mutex> lock(mtx); // Lock the mutex in unique mode
                position = pos;                                // Set the position
            }
            
            int getPrisonTime() const { 
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     int: prison time of the player
                    * Get the prison time of the player
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                return prisonTime;                             // Return the prison time
            }

            void setPrisonTime(int time) { 
                /*
                    * Args:
                    *     int: prison time to set
                    * Returns:
                    *     None
                    * Set the prison time of the player
                 */

                std::unique_lock<std::shared_mutex> lock(mtx); // Lock the mutex in unique mode
                prisonTime = time;                             // Set the prison time
            }
        };

        class ComputerPlayer : Player {
        private:
            std::shared_mutex mtxComputer;

        public:
            // Constructor; initializes the player with the given cash amount.
            ComputerPlayer(cashType initialCash = constant::initialCash) : Player(initialCash) {}
            // Destructor;
            ~ComputerPlayer() override {}
        };
    }

    namespace gamePlay {        
        class Tile {
        public:
            // Enum for the type of the tile
            enum TileType {placeholder, home, buildable, prison, tax, random};

        private:
            TileType type;
            mutable std::shared_mutex mtx;

        public:
            // Constructor; initializes the tile with the given type.
            Tile(TileType _type = placeholder) : type(_type) {}
            // Destructor; virtual to allow derived classes to override.
            virtual ~Tile() {}

            TileType getType() const { 
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     TileType: type of the tile
                    * Get the type of the tile
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                return type;                                   // Return the type
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
            // Constructor; initializes the buildable tile with the given parameters.
            Buildable(cashType _plotCost = constant::defaultPlotCost, cashType _houseCost = constant::defaultHouseCost, 
                      cashType _basicRent = constant::defaultBasicRent, std::array<game::cashType, 6>& _houseRent = constant::defaultHouseRent) 
                    : Tile(buildable), plotCost(_plotCost), houseCost(_houseCost), basicRent(_basicRent), houseRent(_houseRent), owner(nullptr), status({0, 0}) {}
            // Destructor;
            ~Buildable() override {}

            bool isOwned() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     bool: whether the tile is owned
                    * Check if the tile is owned
                 */

                std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock the mutex in shared mode
                return owner != nullptr;                                // Return whether the tile is owned
            }

            player::Player* getOwner() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     player::Player*: owner of the tile
                    * Get the owner of the tile
                 */

                std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock the mutex in shared mode
                return owner;                                           // Return the owner
            }

            void setOwner(player::Player* newOwner) {
                /*
                    * Args:
                    *     player::Player*: new owner to set
                    * Returns:
                    *     None
                    * Set the owner of the tile
                 */

                std::unique_lock<std::shared_mutex> lock(mtxBuildable); // Lock the mutex in unique mode
                owner = newOwner;                                       // Set the owner
            }

            cashType getPlotCost() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     cashType: plot cost of the tile
                    * Get the plot cost of the tile
                 */

                std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock the mutex in shared mode
                return plotCost;                                        // Return the plot cost
            }

            cashType getHouseCost() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     cashType: house cost of the tile
                    * Get the house cost of the tile
                 */

                std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock the mutex in shared mode
                return houseCost;                                       // Return the house cost
            }

            cashType getRent() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     cashType: rent of the tile
                    * Get the rent of the tile
                 */

                std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock the mutex in shared mode
                // Dynamic rent TODO
                cashType rent = basicRent;                              // Set the rent to the basic rent

                if (status.house)
                    rent += houseRent[status.house - 1];                // Add the house rent if there is a house
                return rent;                                            // Return the rent
            }

            buildStatus getStatus() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     buildStatus: status of the tile
                    * Get the status of the tile
                 */

                std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock the mutex in shared mode
                return status;                                          // Return the status
            }

            void setStatus(buildStatus newStatus) {
                /*
                    * Args:
                    *     buildStatus: new status to set
                    * Returns:
                    *     None
                    * Set the status of the tile
                 */

                std::unique_lock<std::shared_mutex> lock(mtxBuildable); // Lock the mutex in unique mode
                status = newStatus;                                     // Set the status
            } 
        };

        // GO tile
        class Home : public Tile {
        private:
            mutable std::shared_mutex mtxHome;

        public:
            Home() : Tile(home) {}
            ~Home() override {}
        };

        // Prison tile
        class Prison : public Tile {
        private:
            mutable std::shared_mutex mtxPrison;

        public:
            Prison() : Tile(prison) {}
            ~Prison() override {}
        };

        // Tax-collecting tile
        class Tax : public Tile {
        private:
            double taxRate;
            mutable std::shared_mutex mtxTax;

        public:
            Tax(double _rate = constant::defaultTaxRate) : Tile(tax), taxRate(_rate) {}
            ~Tax() override {}

            double getTaxRate() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     double: tax rate of the tile
                    * Get the tax rate of the tile
                 */

                std::shared_lock<std::shared_mutex> lock(mtxTax); // Lock the mutex in shared mode
                return taxRate;                                   // Return the tax rate
            }

            void setTaxRate(double newRate) {
                /*
                    * Args:
                    *     double: new tax rate to set
                    * Returns:
                    *     None
                    * Set the tax rate of the tile
                 */

                std::unique_lock<std::shared_mutex> lock(mtxTax); // Lock the mutex in unique mode
                taxRate = newRate;                                // Set the tax rate
            }
        };

        // Random event tile
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
                /* 
                    * Args:
                    *     None
                    * Returns:
                    *     None
                    * Move to the next player
                 */

                currentPlayerIndex = (currentPlayerIndex + 1) % players.size(); // Move to the next player
                utils::Logger::getInstance().log("nextPlayer(): Next player."); // Log the next player
            }

            // Shouldn't be called by the client to avoid mutex deadlock.
            void movePlayer(player::Player* player, int steps) {
                /* 
                    * Args:
                    *     player::Player*: player to move
                    *     int: steps to move
                    * Returns:
                    *     None
                    * Move the player by the given steps
                 */

                int origPosition = player->getPosition();      // Get the original position of the player
                int overPosition = (origPosition + steps);     // Get the new position of the player ignoring the board size
                int newPosition = overPosition % tiles.size(); // Get the new position of the player
                
                // Log the movement
                utils::Logger::getInstance().log("movePlayer(): Player moving from " + std::to_string(origPosition) + " to " + std::to_string(newPosition) + ".");
                
                player->setPosition(newPosition);       // Set the new position of the player
                
                int i = steps - 1;                      // Set the number of steps to move
                auto it = tiles.begin() + origPosition; // Set the iterator to the original position
                if (it == tiles.end())                  // If the iterator is at the end of the tiles
                    it = tiles.begin();                 // Set the iterator to the beginning of the tiles
                while (i--) {
                    // Move the iterator
                    if (++it == tiles.end()) 
                        it = tiles.begin();
                    
                    // Add cash and log if the player passes the GO tile
                    if ((*it)->getType() == Tile::TileType::home) {
                        player->addCash(constant::homeReward);
                        utils::Logger::getInstance().log("movePlayer(): Rewarding player " + std::to_string(constant::homeReward) + " for passing home.");
                        callbackHomeReward(player, static_cast<Home*>(*it), constant::homeReward);
                    }
                }
                
                callbackPlayerUpdate(player);                                           // Update the player
                utils::Logger::getInstance().log("movePlayer(): Handling tile event."); // Log the handling of the tile event
                handleTileEvent(player, tiles[newPosition]);                            // Handle the tile event
            }

            // Shouldn't be called by the client to avoid mutex deadlock.
            void handleTileEvent(player::Player* player, Tile* tile) {
                /* 
                    * Args:
                    *     player::Player*: player to handle the event
                    *     Tile*: tile to handle the event
                    * Returns:
                    *     None
                    * Handle the event of the tile
                 */

                // Handle the event based on the type of the tile
                switch (tile->getType()) {
                    case Tile::buildable: {
                        Buildable* buildableTile = static_cast<Buildable*>(tile);

                        // Buy if not owned
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
                        }

                        // Pay rent if owned by someone else
                        else if (buildableTile->getOwner() != player) {
                            player->addCash(-buildableTile->getRent());
                            buildableTile->getOwner()->addCash(buildableTile->getRent());
                            callbackRent(player, buildableTile, buildableTile->getRent());
                        }

                        // Build if owned by the player
                        else if (buildableTile->getOwner() == player) {
                            // Build
                            // TODO
                        }
                        break;
                    }

                    case Tile::prison:
                        // Go to prison
                        player->setPrisonTime(0);
                        callbackPrison(player, static_cast<Prison*>(tile));
                        break;
                    
                    case Tile::tax: {
                        // Get the tax to pay
                        Tax* taxTile = static_cast<Tax*>(tile);
                        cashType taxToPay = static_cast<cashType>(player->getCash() * taxTile->getTaxRate() / 100.0f) * 100;
                        
                        // Log the tax payment
                        utils::Logger::getInstance().log("handleTileEvent(): Player paid tax " + std::to_string(taxTile->getTaxRate()) + " * " + std::to_string(player->getCash()) + " = " + std::to_string(taxToPay) + ".");
                        
                        // Pay the tax
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
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     GameInstance& instance
                    * Get the instance of the GameInstance class
                 */

                static GameInstance instance;
                return instance;
            }

            const std::vector<Tile*>& getTiles() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     std::vector<Tile*>: tiles
                    * Get the tiles
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                return tiles;                                  // Return the tiles
            }

            const std::vector<player::Player*>& getPlayers() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     std::vector<player::Player*>: players
                    * Get the players
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                return players;                                // Return the players
            }
            
            const int findPlayerPos(player::Player* p) const {
                /*
                    * Args:
                    *     player::Player*: player to find
                    * Returns:
                    *     int: position of the player
                    * Find the position of the player
                 */

                std::shared_lock<std::shared_mutex> lock(mtx);          // Lock the mutex in shared mode
                auto it = std::find(players.begin(), players.end(), p); // Find the player
                
                if (it != players.end()) 
                    return it - players.begin();                        // Return the position of the player
                
                return -1;
            }

            const std::vector<Tile*> findOwnTiles(player::Player* p) const {
                /*
                    * Args:
                    *     player::Player*: player to find the tiles of
                    * Returns:
                    *     std::vector<Tile*>: tiles owned by the player
                    * Find the tiles owned by the player
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                std::vector<Tile*> ownTiles;                   // Initialize the vector of tiles owned by the player

                // Copy the tiles owned by the player to the vector
                std::copy_if(tiles.begin(), tiles.end(), std::back_inserter(ownTiles), [p](Tile* t) {
                    if (t->getType() == Tile::buildable) {
                        return static_cast<Buildable*>(t)->getOwner() == p;
                    }
                    return false;
                });

                return ownTiles;
            }

            const int findNextTile(Tile::TileType type, int pos) const {
                /*
                    * Args:
                    *     Tile::TileType: type of the tile to find
                    *     int: position to start from
                    * Returns:
                    *     int: position of the tile
                    * Find the position of the next tile of the given type
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                auto it = tiles.begin() + pos;                 // Set the iterator to the given position
                
                for (int i = 0; i < tiles.size(); i++) {
                    if (++it == tiles.end()) 
                        it = tiles.begin();
                }

                return -1;
            }

            const int findTile(Tile* tile) const {
                /*
                    * Args:
                    *     Tile*: tile to find
                    * Returns:
                    *     int: position of the tile
                    * Find the position of the tile
                 */

                std::shared_lock<std::shared_mutex> lock(mtx);         // Lock the mutex in shared mode
                auto it = std::find(tiles.begin(), tiles.end(), tile); // Find the tile

                if (it != tiles.end()) 
                    return it - tiles.begin();
                return -1;
            }

            void addTile(Tile* tile) {
                /*
                    * Args:
                    *     Tile*: tile to add
                    * Returns:
                    *     None
                    * Add the tile
                 */

                std::unique_lock<std::shared_mutex> lock(mtx); // Lock the mutex in unique mode
                tiles.push_back(tile);                         // Add the tile
            }

            void addPlayer(player::Player* player) {
                /*
                    * Args:
                    *     player::Player*: player to add
                    * Returns:
                    *     None
                    * Add the player
                 */

                std::unique_lock<std::shared_mutex> lock(mtx); // Lock the mutex in unique mode
                players.push_back(player);                     // Add the player
            }

            static int rollDice(int minimum = constant::diceMinimum, int maximum = constant::diceMaximum) {
                /*
                    * Args:
                    *     int: minimum value of the dice
                    *     int: maximum value of the dice
                    * Returns:
                    *     int: value of the dice
                    * Roll the dice
                 */

                static std::random_device rd;                             // Initialize the random device
                static std::mt19937 gen(rd());                            // Initialize the random number generator
                std::uniform_int_distribution<int> dis(minimum, maximum); // Initialize the uniform distribution
                return dis(gen);                                          // Return the random number
            }

            player::Player* getCurrentPlayer() const {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     player::Player*: current player
                    * Get the current player
                 */

                std::shared_lock<std::shared_mutex> lock(mtx); // Lock the mutex in shared mode
                return players[currentPlayerIndex];            // Return the current player
            }

            void tick() {
                /*
                    * Args:
                    *     None
                    * Returns:
                    *     None
                    * Perform a tick
                 */

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

                // Check for 3 same dice
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