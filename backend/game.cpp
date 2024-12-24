#include "game.h"
#include "utils.h"
#include "constant.h"
#include <random>
#include <algorithm>

using namespace game;
using namespace player;
using namespace gamePlay;

/**
 * @brief Player constructor
 * @param initialCash Initial cash amount for the player
 * @return None
 */
Player::Player(cashType initialCash) : cash(initialCash), position(0), prisonTime(0) {}

/**
 * @brief Player destructor
 * @param None
 * @return None
 */
Player::~Player() {}

/**
 * @brief Get player's current cash amount
 * @param None
 * @return Current cash amount
 */
cashType Player::getCash() {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    return cash;
}

/**
 * @brief Set player's cash amount
 * @param newCash New cash amount to set
 * @return None
 */
void Player::setCash(cashType newCash) {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    cash = newCash;
}

/**
 * @brief Add cash to player's current amount
 * @param delta Amount to add (can be negative)
 * @return New total cash amount
 */
cashType Player::addCash(cashType delta) {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    cash += delta;
    return cash;
}

/**
 * @brief Get player's current position
 * @param None
 * @return Current position on board
 */
int Player::getPosition() const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    return position;
}

/**
 * @brief Set player's position
 * @param pos New position to set
 * @return None
 */
void Player::setPosition(int pos) {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    position = pos;
}

/**
 * @brief Get player's prison time
 * @param None
 * @return Current prison time
 */
int Player::getPrisonTime() const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    return prisonTime;
}

/**
 * @brief Set player's prison time
 * @param time New prison time to set
 * @return None
 */
void Player::setPrisonTime(int time) {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    prisonTime = time;
}

/**
 * @brief Get player's nickname
 * @param None
 * @return Player's nickname
 */
std::string Player::getNickname() const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    return nickname;
}

/**
 * @brief Set player's nickname
 * @param newNickname New nickname to set
 * @return None
 */
void Player::setNickname(const std::string& newNickname) {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    nickname = newNickname;
}

/**
 * @brief ComputerPlayer constructor
 * @param initialCash Initial cash amount for computer player
 * @return None
 */
ComputerPlayer::ComputerPlayer(cashType initialCash) : Player(initialCash) {}

/**
 * @brief ComputerPlayer destructor
 * @param None
 * @return None
 */
ComputerPlayer::~ComputerPlayer() {}

/**
 * @brief Tile constructor
 * @param _type Type of tile
 * @return None
 */
Tile::Tile(TileType _type) : type(_type) {}

/**
 * @brief Tile destructor
 * @param None
 * @return None
 */
Tile::~Tile() {}

/**
 * @brief Get tile type
 * @param None
 * @return Type of tile
 */
Tile::TileType Tile::getType() const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    return type;
}

/**
 * @brief Buildable constructor
 * @param _plotCost Cost of plot
 * @param _houseCost Cost of house
 * @param _basicRent Basic rent amount
 * @param _houseRent Array of rent amounts for different house levels
 * @return None
 */
Buildable::Buildable(cashType _plotCost, cashType _houseCost, cashType _basicRent, std::array<cashType, 6>& _houseRent)
    : Tile(buildable), plotCost(_plotCost), houseCost(_houseCost), basicRent(_basicRent), houseRent(_houseRent), owner(nullptr), status(empty) {
        static int colorCounter = 0;
        color = (colorCounter++) % 3;
    }

/**
 * @brief Buildable destructor
 * @param None
 * @return None
 */
Buildable::~Buildable() {}

/**
 * @brief Check if property is owned
 * @param None
 * @return True if owned, false otherwise
 */
bool Buildable::isOwned() const {
    std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    return owner != nullptr;
}

/**
 * @brief Get property owner
 * @param None
 * @return Pointer to owner player
 */
Player* Buildable::getOwner() const {
    std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    return owner;
}

/**
 * @brief Set property owner
 * @param newOwner Pointer to new owner player
 * @return None
 */
void Buildable::setOwner(Player* newOwner) {
    std::unique_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    owner = newOwner;
}

/**
 * @brief Get plot cost
 * @param None
 * @return Cost of plot
 */
cashType Buildable::getPlotCost() const {
    std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    return plotCost;
}

/**
 * @brief Get house cost
 * @param None
 * @return Cost of house
 */
cashType Buildable::getHouseCost() const {
    std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    return houseCost;
}

/**
 * @brief Get current rent amount
 * @param None
 * @return Current rent amount based on development status
 */
cashType Buildable::getRent() const {
    std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    cashType rent = basicRent;
    if (status)
        rent += houseRent[status - 1];
    return rent;
}

/**
 * @brief Get build status
 * @param None
 * @return Current build status
 */
Buildable::buildStatus Buildable::getStatus() const {
    std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    return status;
}

/**
 * @brief Set build status
 * @param newStatus New build status to set
 * @return None
 */
void Buildable::setStatus(buildStatus newStatus) {
    std::unique_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    status = newStatus;
}

/**
 * @brief Get color
 * @param None
 * @return Color
 */
int Buildable::getColor() const {
    std::shared_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    return color;
}

/**
 * @brief Set color
 * @param newColor New color to set
 * @return None
 */
void Buildable::setColor(int newColor) {
    std::unique_lock<std::shared_mutex> lock(mtxBuildable); // Lock for thread-safe access
    color = newColor;
}

/**
 * @brief Home constructor
 * @param None
 * @return None
 */
Home::Home() : Tile(home) {}

/**
 * @brief Home destructor
 * @param None
 * @return None
 */
Home::~Home() {}

/**
 * @brief Prison constructor
 * @param None
 * @return None
 */
Prison::Prison() : Tile(prison) {}

/**
 * @brief Prison destructor
 * @param None
 * @return None
 */
Prison::~Prison() {}

/**
 * @brief Tax constructor
 * @param _rate Tax rate
 * @return None
 */
Tax::Tax(double _rate) : Tile(tax), taxRate(_rate) {}

/**
 * @brief Tax destructor
 * @param None
 * @return None
 */
Tax::~Tax() {}

/**
 * @brief Get tax rate
 * @param None
 * @return Current tax rate
 */
double Tax::getTaxRate() const {
    std::shared_lock<std::shared_mutex> lock(mtxTax); // Lock for thread-safe access
    return taxRate;
}

/**
 * @brief Set tax rate
 * @param newRate New tax rate to set
 * @return None
 */
void Tax::setTaxRate(double newRate) {
    std::unique_lock<std::shared_mutex> lock(mtxTax); // Lock for thread-safe access
    taxRate = newRate; // Set the new tax rate
}

/**
 * @brief Random constructor
 * @param None
 * @return None
 */
Random::Random() : Tile(random) {}

/**
 * @brief Random destructor
 * @param None
 * @return None
 */
Random::~Random() {}

/**
 * @brief GameInstance constructor
 * @param None
 * @return None
 */
GameInstance::GameInstance() : currentPlayerIndex(0) {
    utils::Logger::getInstance(); // Initialize logger
}

/**
 * @brief GameInstance destructor
 * @param None
 * @return None
 */
GameInstance::~GameInstance() {
    for (auto& t : tiles) 
        if (t != nullptr) 
            delete t; // Delete all tiles
    for (auto& p : players) 
        if (p != nullptr) 
            delete p; // Delete all players
}

/**
 * @brief Notifies the game instance that user input is ready
 * @param User input value
 * @return None 
 */
void GameInstance::notifyUserInput(const std::any& result) {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    userInputResult = result; // Set user input result
    userInputReady = true; // Set user input ready flag
    cv.notify_one(); // Notify waiting thread
}

/**
 * @brief Called by the game instance to wait for user input
 * @param None
 * @return User input value
 */
std::any GameInstance::waitForUserInput() {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    cv.wait(lock, [this] { return userInputReady; }); // Wait for user input
    userInputReady = false; // Reset user input ready flag
    return userInputResult; // Return user input result
}

/**
 * @brief Get singleton instance
 * @param None
 * @return Reference to GameInstance singleton
 */
GameInstance& GameInstance::getInstance() {
    static GameInstance instance; // Singleton instance
    return instance;
}

/**
 * @brief Get tiles vector
 * @param None
 * @return Const reference to tiles vector
 */
const std::vector<Tile*>& GameInstance::getTiles() const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    return tiles; // Return tiles vector
}

/**
 * @brief Get players vector
 * @param None
 * @return Const reference to players vector
 */
const std::vector<Player*>& GameInstance::getPlayers() const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    return players; // Return players vector
}

/**
 * @brief Find player position in players vector
 * @param p Player pointer to find
 * @return Index of player or -1 if not found
 */
const int GameInstance::findPlayerPos(Player* p) const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    auto it = std::find(players.begin(), players.end(), p); // Find player in vector
    if (it != players.end())
        return it - players.begin(); // Return index if found
    return -1; // Return -1 if not found
}

/**
 * @brief Find tiles owned by player
 * @param p Player pointer
 * @return Vector of tiles owned by player
 */
const std::vector<Tile*> GameInstance::findOwnTiles(Player* p) const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    std::vector<Tile*> ownTiles;
    std::copy_if(tiles.begin(), tiles.end(), std::back_inserter(ownTiles), [p](Tile* t) {
        if (t->getType() == Tile::buildable) {
            return static_cast<Buildable*>(t)->getOwner() == p; // Check if tile is owned by player
        }
        return false;
    });
    return ownTiles; // Return vector of owned tiles
}

/**
 * @brief Find next tile of specified type
 * @param type Type of tile to find
 * @param pos Starting position
 * @return Position of next matching tile or -1 if not found
 */
const int GameInstance::findNextTile(Tile::TileType type, int pos) const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    auto it = tiles.begin() + pos; // Start from the given position
    for (int i = 0; i < tiles.size(); i++) {
        if (++it == tiles.end())
            it = tiles.begin(); // Wrap around if at the end
        if ((*it)->getType() == type)
            return it - tiles.begin(); // Return position if type matches
    }
    return -1; // Return -1 if not found
}

/**
 * @brief Find position of specific tile
 * @param tile Tile pointer to find
 * @return Position of tile or -1 if not found
 */
const int GameInstance::findTile(Tile* tile) const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    auto it = std::find(tiles.begin(), tiles.end(), tile); // Find tile in vector
    if (it != tiles.end())
        return it - tiles.begin(); // Return position if found
    return -1; // Return -1 if not found
}

/**
 * @brief Add tile to board
 * @param tile Tile pointer to add
 * @return None
 */
void GameInstance::addTile(Tile* tile) {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    tiles.push_back(tile); // Add tile to vector
}

/**
 * @brief Add player to game
 * @param player Player pointer to add
 * @return None
 */
void GameInstance::addPlayer(Player* player) {
    std::unique_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    players.push_back(player); // Add player to vector
}

/**
 * @brief Roll dice
 * @param minimum Minimum value
 * @param maximum Maximum value
 * @return Random number between minimum and maximum
 */
int GameInstance::rollDice(int minimum, int maximum) {
    static std::random_device rd; // Random device for seeding
    static std::mt19937 gen(rd()); // Mersenne Twister generator
    std::uniform_int_distribution<int> dis(minimum, maximum); // Uniform distribution
    return dis(gen); // Generate and return random number
}

/**
 * @brief Get current player
 * @param None
 * @return Pointer to current player
 */
Player* GameInstance::getCurrentPlayer() const {
    std::shared_lock<std::shared_mutex> lock(mtx); // Lock for thread-safe access
    return players[currentPlayerIndex]; // Return current player
}

/**
 * @brief Process game tick
 * @param None
 * @return None
 */
void GameInstance::tick() {
    utils::Logger::getInstance().log("tick(): Trying to acquire lock...");

    Player* currentPlayer;
    int currentPosition;
    Tile::TileType currentTileType;

    {
        std::shared_lock<std::shared_mutex> lock(mtx); // Acquire shared lock for thread-safe access
        currentPlayer = getCurrentPlayer(); // Get the current player
        currentPosition = currentPlayer->getPosition(); // Get the current position of the player
        currentTileType = tiles[currentPosition]->getType(); // Get the type of the current tile
    }

    utils::Logger::getInstance().log("tick(): Lock released.");
    utils::Logger::getInstance().log("tick(): Tick start. Player: " + std::to_string(currentPlayerIndex) + ", Position: " + std::to_string(currentPosition) + ".");

    if (currentTileType == Tile::TileType::prison) { // Check if the player is in prison
        utils::Logger::getInstance().log("tick(): Player is in prison.");

        if (currentPlayer->getPrisonTime() < 3) { // Check if the player has been in prison for less than 3 turns
            for (int i = 0; i < 3; i++) {
                utils::Logger::getInstance().log("tick(): Rolling dice for prison, try: " + std::to_string(i) + ".");
                int diceValue1 = rollDice(), diceValue2 = rollDice(); // Roll two dice
                callbackDice(currentPlayer, diceValue1, diceValue2); // Callback for dice roll
                if (diceValue1 == diceValue2) { // Check if the player rolled a double
                    utils::Logger::getInstance().log("tick(): Player successfully got out of prison.");
                    movePlayer(currentPlayer, diceValue1 + diceValue2); // Move the player
                    nextPlayer(); // Advance to the next player
                    return;
                }
            }

            callbackPrisonPayOut(currentPlayer, static_cast<Prison*>(tiles[currentPosition]), constant::prisonReleasePrice[currentPlayer->getPrisonTime()]);
            if (currentPlayer->getCash() >= constant::prisonReleasePrice[currentPlayer->getPrisonTime()] 
                && std::any_cast<bool>(waitForUserInput())) {
                utils::Logger::getInstance().log("tick(): Player paid to get out of prison.");
                currentPlayer->addCash(-constant::prisonReleasePrice[currentPlayer->getPrisonTime()]); // Deduct prison release price from player
                currentPlayer->setPrisonTime(0); // Reset prison time
            } else {
                utils::Logger::getInstance().log("tick(): Player failed to get out of prison.");
                currentPlayer->setPrisonTime(currentPlayer->getPrisonTime() + 1); // Increment prison time
                nextPlayer(); // Advance to the next player
                return;
            }
        }
    }

    int diceValue1 = rollDice(), diceValue2 = rollDice(); // Roll two dice
    utils::Logger::getInstance().log("tick(): Player rolled dice " + std::to_string(diceValue1) + ", " + std::to_string(diceValue2) + ".");
    callbackDice(currentPlayer, diceValue1, diceValue2); // Callback for dice roll

    if (diceValue1 == diceValue2) { // Check if the player rolled a double
        int diceValue3 = rollDice(); // Roll a third dice
        utils::Logger::getInstance().log("tick(): Player rolling a third dice " + std::to_string(diceValue3) + ".");
        callbackDice3rd(currentPlayer, diceValue3); // Callback for third dice roll

        if (diceValue3 == diceValue1) { // Check if the third dice matches the first two
            int prisonPos = findNextTile(Tile::TileType::prison, currentPosition); // Find the next prison tile

            if (prisonPos != -1) {
                currentPlayer->setPosition(prisonPos); // Move the player to the prison tile
                currentPlayer->setPrisonTime(0); // Reset prison time
                utils::Logger::getInstance().log("tick(): Player prisoned for 3 same dice.");
                callbackPrison(currentPlayer, static_cast<Prison*>(tiles[prisonPos])); // Callback for prison event
            } else {
                utils::Logger::getInstance().log("tick(): No prison found.");
            }
        } else {
            movePlayer(currentPlayer, diceValue1 + diceValue2 + diceValue3); // Move the player
        }
    } else {
        movePlayer(currentPlayer, diceValue1 + diceValue2); // Move the player
    }

    nextPlayer(); // Advance to the next player
}
/**
 * @brief Advances the game to the next player.
 */
void GameInstance::nextPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size(); // Move to the next player
    utils::Logger::getInstance().log("nextPlayer(): Next player.");
}

/**
 * @brief Moves a player a specified number of steps on the game board.
 * @param player The player to move.
 * @param steps The number of steps to move the player.
 * @return None
 */
void GameInstance::movePlayer(Player* player, int steps) {
    int origPosition = player->getPosition(); // Get the original position of the player
    int overPosition = (origPosition + steps); // Calculate the new position before wrapping around
    int newPosition = overPosition % tiles.size(); // Wrap around the board if necessary

    utils::Logger::getInstance().log("movePlayer(): Player moving from " + std::to_string(origPosition) + " to " + std::to_string(newPosition) + ".");

    player->setPosition(newPosition); // Set the new position of the player

    int i = steps - 1;
    auto it = tiles.begin() + origPosition; // Iterator to the original position
    if (it == tiles.end())
        it = tiles.begin(); // Wrap around if at the end
    while (i--) {
        if (++it == tiles.end())
            it = tiles.begin(); // Wrap around if at the end

        if ((*it)->getType() == Tile::TileType::home) { // Check if the tile is a home tile
            player->addCash(constant::homeReward); // Reward the player for passing home
            utils::Logger::getInstance().log("movePlayer(): Rewarding player " + std::to_string(constant::homeReward) + " for passing home.");
            callbackHomeReward(player, static_cast<Home*>(*it), constant::homeReward); // Callback for home reward
        }
    }

    callbackPlayerUpdate(player); // Callback for player update
    utils::Logger::getInstance().log("movePlayer(): Handling tile event.");
    handleTileEvent(player, tiles[newPosition]); // Handle the event for the new tile
}

/**
 * @brief Handles the event that occurs when a player lands on a specific tile.
 * @param player The player who landed on the tile.
 * @param tile The tile the player landed on.
 */
void GameInstance::handleTileEvent(Player* player, Tile* tile) {
    switch (tile->getType()) {
        case Tile::buildable: {
            Buildable* buildableTile = static_cast<Buildable*>(tile);

            if (!buildableTile->isOwned()) { // If the tile is not owned
                callbackBuy(player, buildableTile, buildableTile->getPlotCost());
                if (player->getCash() >= buildableTile->getPlotCost() && std::any_cast<bool>(waitForUserInput())) {
                    player->setCash(player->getCash() - buildableTile->getPlotCost()); // Deduct plot cost from player
                    buildableTile->setOwner(player); // Set player as owner
                } else {
                    callbackAuction(buildableTile, constant::auctionReservePrice, constant::auctionBidIncrement);
                    auctionResult result = std::any_cast<auctionResult>(waitForUserInput());
                    if (result.price >= constant::auctionReservePrice) {
                        result.player->addCash(-result.price); // Deduct auction price from winner
                        buildableTile->setOwner(result.player); // Set auction winner as owner
                    }
                }
            } else if (buildableTile->getOwner() != player) { // If the tile is owned by another player
                player->addCash(-buildableTile->getRent()); // Deduct rent from player
                buildableTile->getOwner()->addCash(buildableTile->getRent()); // Add rent to owner
                callbackRent(player, buildableTile, buildableTile->getRent()); // Callback for rent payment
            } else if (buildableTile->getOwner() == player) {
                // Build
                // TODO
            }
            break;
        }

        case Tile::prison:
            player->setPrisonTime(0); // Set prison time to 0
            callbackPrison(player, static_cast<Prison*>(tile)); // Callback for prison event
            break;

        case Tile::tax: {
            Tax* taxTile = static_cast<Tax*>(tile);
            cashType taxToPay = static_cast<cashType>(player->getCash() * taxTile->getTaxRate() / 100.0f) * 100; // Calculate tax to pay
            utils::Logger::getInstance().log("handleTileEvent(): Player paid tax " + std::to_string(taxTile->getTaxRate()) + " * " + std::to_string(player->getCash()) + " = " + std::to_string(taxToPay) + ".");
            player->addCash(-taxToPay); // Deduct tax from player
            callbackTax(player, taxTile, taxToPay); // Callback for tax payment
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