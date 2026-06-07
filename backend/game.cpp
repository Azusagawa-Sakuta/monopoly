#include "game.h"
#include "utils.h"
#include "constant.h"
#include <random>
#include <algorithm>
#include <QString>
#include <QTimer>

using namespace game;
using namespace player;
using namespace gamePlay;

// ─── Player ───────────────────────────────────────────────────────────────────

Player::Player(cashType initialCash) : cash(initialCash), position(0), prisonTime(0), bankrupted(false) {}

Player::~Player() {}

cashType Player::getCash() { return cash; }

void Player::setCash(cashType newCash) { cash = newCash; }

cashType Player::addCash(cashType delta) {
    cash += delta;
    return cash;
}

int Player::getPosition() const { return position; }

void Player::setPosition(int pos) { position = pos; }

int Player::getPrisonTime() const { return prisonTime; }

void Player::setPrisonTime(int time) { prisonTime = time; }

std::string Player::getNickname() const { return nickname; }

void Player::setNickname(const std::string& newNickname) { nickname = newNickname; }

std::string Player::getImagePath() const { return imagePath; }

void Player::setImagePath(const std::string& newImagePath) { imagePath = newImagePath; }

bool Player::isBankrupted() const { return bankrupted; }

void Player::setBankrupted(bool newBankrupted) { bankrupted = newBankrupted; }

bool Player::isComputer() const { return false; }

// ─── ComputerPlayer ───────────────────────────────────────────────────────────

ComputerPlayer::ComputerPlayer(cashType initialCash) : Player(initialCash) {}

ComputerPlayer::~ComputerPlayer() {}

bool ComputerPlayer::isComputer() const { return true; }

// ─── Tile ─────────────────────────────────────────────────────────────────────

Tile::Tile(TileType _type) : type(_type) {}

Tile::~Tile() {}

Tile::TileType Tile::getType() const { return type; }

// ─── Buildable ────────────────────────────────────────────────────────────────

Buildable::Buildable(cashType _plotCost, cashType _houseCost, cashType _basicRent, std::array<cashType, 6>& _houseRent)
    : Tile(buildable), plotCost(_plotCost), houseCost(_houseCost), basicRent(_basicRent), houseRent(_houseRent), owner(nullptr), status(empty) {
    static int colorCounter = 0;
    color = colorCounter++;
    colorCounter %= 4;
}

Buildable::~Buildable() {}

bool Buildable::isOwned() const { return owner != nullptr; }

Player* Buildable::getOwner() const { return owner; }

void Buildable::setOwner(Player* newOwner) { owner = newOwner; }

cashType Buildable::getPlotCost() const { return plotCost; }

cashType Buildable::getHouseCost() const { return houseCost; }

cashType Buildable::getRent() const {
    cashType rent = basicRent;
    if (status)
        rent += houseRent[status - 1];
    return rent;
}

Buildable::buildStatus Buildable::getStatus() const { return status; }

void Buildable::setStatus(buildStatus newStatus) { status = newStatus; }

int Buildable::getColor() const { return color; }

void Buildable::setColor(int newColor) { color = newColor; }

cashType Buildable::getValue() const {
    cashType value = plotCost;
    if (status)
        value += houseCost * status;
    return value;
}

// ─── Home / Prison / Tax / Random ─────────────────────────────────────────────

Home::Home() : Tile(home) {}
Home::~Home() {}

Prison::Prison() : Tile(prison) {}
Prison::~Prison() {}

Tax::Tax(double _rate) : Tile(tax), taxRate(_rate) {}
Tax::~Tax() {}

double Tax::getTaxRate() const { return taxRate; }

void Tax::setTaxRate(double newRate) { taxRate = newRate; }

Random::Random() : Tile(random) {}
Random::~Random() {}

// ─── GameInstance ─────────────────────────────────────────────────────────────

GameInstance::GameInstance()
    : currentPlayerIndex(0), currentStep(GameStep::Idle),
      savedPlayer(nullptr), savedTile(nullptr), savedAmount(0),
      returnStep(GameStep::Idle) {
    utils::Logger::getInstance();
}

GameInstance::~GameInstance() {
    for (auto& t : tiles)
        if (t != nullptr)
            delete t;
    for (auto& p : players)
        if (p != nullptr)
            delete p;
}

GameInstance& GameInstance::getInstance() {
    static GameInstance instance;
    return instance;
}

const std::vector<Tile*>& GameInstance::getTiles() const { return tiles; }

const std::vector<Player*>& GameInstance::getPlayers() const { return players; }

int GameInstance::findPlayerIndex(Player* p) const {
    auto it = std::find(players.begin(), players.end(), p);
    if (it != players.end())
        return it - players.begin();
    return -1;
}

const std::vector<Tile*> GameInstance::findOwnTiles(Player* p) const {
    std::vector<Tile*> ownTiles;
    std::copy_if(tiles.begin(), tiles.end(), std::back_inserter(ownTiles), [p](Tile* t) {
        if (t->getType() == Tile::buildable) {
            return static_cast<Buildable*>(t)->getOwner() == p;
        }
        return false;
    });
    return ownTiles;
}

int GameInstance::findNextTile(Tile::TileType type, int pos) const {
    auto it = tiles.begin() + pos;
    for (size_t i = 0; i < tiles.size(); i++) {
        if (++it == tiles.end())
            it = tiles.begin();
        if ((*it)->getType() == type)
            return it - tiles.begin();
    }
    return -1;
}

int GameInstance::findTile(Tile* tile) const {
    auto it = std::find(tiles.begin(), tiles.end(), tile);
    if (it != tiles.end())
        return it - tiles.begin();
    return -1;
}

void GameInstance::addTile(Tile* tile) { tiles.push_back(tile); }

void GameInstance::addPlayer(Player* player) { players.push_back(player); }

Player* GameInstance::getCurrentPlayer() const { return players[currentPlayerIndex]; }

// ─── State Machine ────────────────────────────────────────────────────────────
// Internal sub-step counter for loops (prison dice attempts)
static int prisonAttempt;

void GameInstance::advance() {
    utils::Logger::getInstance().log("advance(): step=" + std::to_string(static_cast<int>(currentStep)));

    switch (currentStep) {

    // ── Idle: start a new turn ─────────────────────────────────────────
    case GameStep::Idle: {
        // Check win conditions
        int notBankrupt = 0;
        Player* winner = nullptr;
        for (const auto& p : players) {
            if (p->getCash() > 500000) {
                QTimer::singleShot(0, this, [this, p]() {
                    emit gameEnded(p);
                });
                currentStep = GameStep::GameOver;
                return;
            }
            if (!p->isBankrupted()) {
                notBankrupt++;
                winner = p;
            }
        }
        if (notBankrupt == 1) {
            QTimer::singleShot(0, this, [this, winner]() {
                emit gameEnded(winner);
            });
            currentStep = GameStep::GameOver;
            return;
        }

        Player* cp = getCurrentPlayer();
        emit turnStarted(cp);
        emit boardUpdateNeeded();

        // Check if current player is in prison
        if (tiles[cp->getPosition()]->getType() == Tile::prison && cp->getPrisonTime() < 3) {
            prisonAttempt = 0;
            currentStep = GameStep::WaitingPrisonDice;
            emit prisonDiceNeeded(cp);
            return;
        }

        // Auto-release after 3 turns in prison
        if (tiles[cp->getPosition()]->getType() == Tile::prison && cp->getPrisonTime() >= 3) {
            cp->setPrisonTime(0);
            utils::Logger::getInstance().log("advance(): Auto-released from prison after 3 turns.");
        }

        currentStep = GameStep::AnimatingDice;
        emit diceRolled(cp, 0, 0);  // UI handles dice animation + provides sum
        return;
    }

    // ── RollingDice → transition to dice animation ─────────────────────
    case GameStep::RollingDice: {
        Player* cp = getCurrentPlayer();
        currentStep = GameStep::AnimatingDice;
        emit diceRolled(cp, 0, 0);
        return;
    }

    // ── WaitingPrisonDice: resume with dice result from UI ─────────────
    // Handled entirely in provideInput()

    // ── WaitingPrisonPayOut: resume with pay decision ──────────────────
    // Handled entirely in provideInput()

    // ── AnimatingDice → after dice input, move player ──────────────────
    // Handled in provideInput() which sets currentStep back to here
    // The continuation after dice roll is handled inline in provideInput

    // ── Moving player (called from provideInput after dice) ────────────
    // This is handled inline in provideInput's dice continuation

    // ── InTileEvent → handle tile effect ───────────────────────────────
    case GameStep::HandlingTile: {
        handleTileEvent(savedPlayer, savedTile);
        break;
    }

    // ── Waiting states: do nothing, waiting for provideInput ───────────
    case GameStep::WaitingBuyDecision:
    case GameStep::WaitingAuction:
    case GameStep::WaitingBuild:
    case GameStep::WaitingSell:
    case GameStep::WaitingUpdate:
    case GameStep::AnimatingDice:
    case GameStep::WaitingPrisonDice:
    case GameStep::WaitingPrisonPayOut:
    case GameStep::WaitingHomeReward:
        // Waiting for UI input — nothing to do
        break;

    // ── TurnEnd → next player and loop ─────────────────────────────────
    case GameStep::TurnEnd:
        nextPlayer();
        currentStep = GameStep::Idle;
        advance();
        break;

    // ── GameOver → stop ────────────────────────────────────────────────
    case GameStep::GameOver:
        break;

    // ── MovingPlayer → continue after UI update ────────────────────────
    case GameStep::MovingPlayer:
        // movePlayer has finished, now handle the tile
        currentStep = GameStep::HandlingTile;
        handleTileEvent(getCurrentPlayer(), tiles[getCurrentPlayer()->getPosition()]);
        break;
    }
}

void GameInstance::provideInput(const std::any& result) {
    utils::Logger::getInstance().log("provideInput(): step=" + std::to_string(static_cast<int>(currentStep)));

    switch (currentStep) {

    // ── Prison dice result ─────────────────────────────────────────────
    case GameStep::WaitingPrisonDice: {
        int diceValue = std::any_cast<int>(result);
        Player* cp = getCurrentPlayer();

        if (diceValue != 0) {
            // Successfully rolled out of prison
            utils::Logger::getInstance().log("provideInput(): Player escaped prison with dice.");
            movePlayer(cp, diceValue);
            // movePlayer emits boardUpdateNeeded, which triggers the full signal
            // chain (handleTileEvent -> ... -> TurnEnd -> advance). Always return
            // here - the chain handles advancing to the next player.
            return;
        }

        // Failed this attempt
        prisonAttempt++;
        if (prisonAttempt < 3) {
            currentStep = GameStep::WaitingPrisonDice;
            emit prisonDiceNeeded(cp);
            return;
        }

        // All 3 attempts failed — offer to pay
        if (cp->getCash() >= constant::prisonReleasePrice[cp->getPrisonTime()]) {
            currentStep = GameStep::WaitingPrisonPayOut;
            savedAmount = constant::prisonReleasePrice[cp->getPrisonTime()];
            emit prisonPayOutNeeded(cp, savedAmount);
            return;
        }

        // Can't pay — stay in prison
        utils::Logger::getInstance().log("provideInput(): Player cannot afford prison release.");
        cp->setPrisonTime(cp->getPrisonTime() + 1);
        nextPlayer();
        currentStep = GameStep::Idle;
        advance();
        break;
    }

    // ── Prison pay decision ────────────────────────────────────────────
    case GameStep::WaitingPrisonPayOut: {
        bool pay = std::any_cast<bool>(result);
        Player* cp = getCurrentPlayer();

        if (pay) {
            utils::Logger::getInstance().log("provideInput(): Player paid to leave prison.");
            cp->addCash(-constant::prisonReleasePrice[cp->getPrisonTime()]);
            cp->setPrisonTime(0);
        } else {
            utils::Logger::getInstance().log("provideInput(): Player chose not to pay.");
            cp->setPrisonTime(cp->getPrisonTime() + 1);
            nextPlayer();
            currentStep = GameStep::Idle;
            advance();
        }
        break;
    }

    // ── Dice roll result ───────────────────────────────────────────────
    case GameStep::AnimatingDice: {
        int diceValue = std::any_cast<int>(result);
        utils::Logger::getInstance().log("provideInput(): Dice result=" + std::to_string(diceValue));

        Player* cp = getCurrentPlayer();

        if (diceValue < 0) {
            // Three same dice → go to prison
            int prisonPos = findNextTile(Tile::prison, cp->getPosition());
            if (prisonPos != -1) {
                cp->setPosition(prisonPos);
                cp->setPrisonTime(0);
                utils::Logger::getInstance().log("provideInput(): Player sent to prison for 3 same dice.");
                Prison* pri = static_cast<Prison*>(tiles[prisonPos]);
                currentStep = GameStep::TurnEnd;
                emit playerPrisoned(cp, pri);
                emit boardUpdateNeeded();
                advance();
            } else {
                utils::Logger::getInstance().log("provideInput(): No prison found on board.");
                nextPlayer();
                currentStep = GameStep::Idle;
                advance();
            }
            return;
        }

        movePlayer(cp, std::abs(diceValue));
        // movePlayer emits boardUpdateNeeded, which triggers the full signal
        // chain (handleTileEvent -> ... -> TurnEnd -> advance). Always return
        // here - the chain handles advancing to the next player.
        break;
    }

    // ── Home reward displayed ──────────────────────────────────────────
    case GameStep::WaitingHomeReward: {
        // UI acknowledged the reward display; continue movement
        // The movement continues in the inline movePlayer logic
        // This is handled by the movement loop in movePlayer
        // For now, continue to the update step
        currentStep = GameStep::WaitingUpdate;
        emit boardUpdateNeeded();
        // UI will call provideInput again after updating
        break;
    }

    // ── UI updated after movement ──────────────────────────────────────
    case GameStep::WaitingUpdate: {
        // UI has updated the board; now handle the tile event
        currentStep = GameStep::HandlingTile;
        handleTileEvent(getCurrentPlayer(), tiles[getCurrentPlayer()->getPosition()]);
        break;
    }

    // ── Buy decision ───────────────────────────────────────────────────
    case GameStep::WaitingBuyDecision: {
        bool buy = std::any_cast<bool>(result);
        Buildable* buildableTile = static_cast<Buildable*>(savedTile);
        Player* cp = getCurrentPlayer();

        if (buy) {
            cp->setCash(cp->getCash() - buildableTile->getPlotCost());
            buildableTile->setOwner(cp);
            emit propertyChanged(buildableTile);
            emit playerUpdated(cp);
            currentStep = GameStep::TurnEnd;
            advance();
        } else {
            // Start auction
            currentStep = GameStep::WaitingAuction;
            savedTile = buildableTile;
            emit auctionNeeded(buildableTile, constant::auctionReservePrice, constant::auctionBidIncrement);
        }
        break;
    }

    // ── Auction result ─────────────────────────────────────────────────
    case GameStep::WaitingAuction: {
        auctionResult ar = std::any_cast<auctionResult>(result);
        Buildable* buildableTile = static_cast<Buildable*>(savedTile);

        if (ar.price >= constant::auctionReservePrice && ar.player != nullptr) {
            ar.player->addCash(-ar.price);
            buildableTile->setOwner(ar.player);
            emit propertyChanged(buildableTile);
            emit playerUpdated(ar.player);
        }
        currentStep = GameStep::TurnEnd;
        advance();
        break;
    }

    // ── Build decision ─────────────────────────────────────────────────
    case GameStep::WaitingBuild: {
        Buildable::buildStatus newStatus = std::any_cast<Buildable::buildStatus>(result);
        Buildable* buildableTile = static_cast<Buildable*>(savedTile);
        Player* cp = getCurrentPlayer();

        if (newStatus > buildableTile->getStatus()) {
            int levels = newStatus - buildableTile->getStatus();
            cp->addCash(-buildableTile->getHouseCost() * levels);
            buildableTile->setStatus(newStatus);
            emit propertyChanged(buildableTile);
            emit playerUpdated(cp);
        }
        currentStep = GameStep::TurnEnd;
        advance();
        break;
    }

    // ── Sell decision ──────────────────────────────────────────────────
    case GameStep::WaitingSell: {
        auto toSell = std::any_cast<std::vector<Buildable*>>(result);
        Player* cp = getCurrentPlayer();
        Buildable* creditorTile = static_cast<Buildable*>(savedTile);
        cashType rentOwed = savedAmount;

        if (toSell.empty()) {
            // Player goes bankrupt
            utils::Logger::getInstance().log("provideInput(): Player bankrupted.");
            cashType total = cp->getCash();
            for (auto& t : findOwnTiles(cp)) {
                if (t->getType() == Tile::buildable) {
                    total += static_cast<Buildable*>(t)->getValue();
                    static_cast<Buildable*>(t)->setOwner(nullptr);
                    static_cast<Buildable*>(t)->setStatus(Buildable::empty);
                    emit propertyChanged(static_cast<Buildable*>(t));
                }
            }
            creditorTile->getOwner()->addCash(total);
            cp->setCash(0);
            cp->setBankrupted(true);
            QTimer::singleShot(0, this, [this, cp, creditorTile]() {
                emit playerBankrupted(cp);
                emit playerUpdated(creditorTile->getOwner());
            });
        } else {
            utils::Logger::getInstance().log("provideInput(): Player sold " + std::to_string(toSell.size()) + " tiles.");
            cashType totalSellValue = 0;
            for (auto& t : toSell) {
                totalSellValue += t->getValue() / 2 / 100 * 100;
                t->setOwner(nullptr);
                t->setStatus(Buildable::empty);
                emit propertyChanged(t);
            }
            cp->addCash(totalSellValue);

            // Now pay the rent
            cp->addCash(-rentOwed);
            creditorTile->getOwner()->addCash(rentOwed);
            QTimer::singleShot(0, this, [this, cp]() {
                emit playerUpdated(cp);
            });
            QTimer::singleShot(0, this, [this, creditorTile]() {
                emit playerUpdated(creditorTile->getOwner());
            });
            QTimer::singleShot(0, this, [this, cp, creditorTile, rentOwed]() {
                emit rentPaid(cp, creditorTile->getOwner(), creditorTile, rentOwed);
            });
        }
        currentStep = GameStep::TurnEnd;
        // // Change the State before update board, or causing any_cast wrong;
        // QTimer::singleShot(0, this, [this]() {
        //     emit boardUpdateNeeded();
        // });
        advance();
        break;
    }

    default:
        utils::Logger::getInstance().log("provideInput(): Unhandled step " + std::to_string(static_cast<int>(currentStep)));
        break;
    }
}

// ─── Game Logic ───────────────────────────────────────────────────────────────

void GameInstance::nextPlayer() {
    do
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    while (players[currentPlayerIndex]->isBankrupted());
    utils::Logger::getInstance().log("nextPlayer(): Next player index=" + std::to_string(currentPlayerIndex));
}

void GameInstance::movePlayer(Player* player, int steps) {
    int origPosition = player->getPosition();
    int overPosition = origPosition + steps;
    int newPosition = overPosition % tiles.size();

    utils::Logger::getInstance().log("movePlayer(): Player moving from " + std::to_string(origPosition) + " to " + std::to_string(newPosition) + ".");

    player->setPosition(newPosition);
    emit playerMoved(player, origPosition, newPosition);

    // Check for passing home along the way
    int remaining = steps;
    auto it = tiles.begin() + origPosition;
    if (it == tiles.end())
        it = tiles.begin();

    while (remaining > 0) {
        remaining--;
        if (++it == tiles.end())
            it = tiles.begin();

        if ((*it)->getType() == Tile::home) {
            player->addCash(constant::homeReward);
            utils::Logger::getInstance().log("movePlayer(): Passed home, rewarding " + std::to_string(constant::homeReward));
            emit playerPassedGo(player, constant::homeReward);
            emit playerUpdated(player);
        }
    }

    // Request UI update before handling tile
    savedPlayer = player;
    savedTile = tiles[newPosition];
    currentStep = GameStep::WaitingUpdate;
    emit boardUpdateNeeded();
}

void GameInstance::handleTileEvent(Player* player, Tile* tile) {
    utils::Logger::getInstance().log("handleTileEvent(): Player=" + std::to_string(findPlayerIndex(player)) + " tile=" + std::to_string(findTile(tile)));

    emit tileLanded(player, tile);

    switch (tile->getType()) {

    case Tile::buildable: {
        Buildable* buildableTile = static_cast<Buildable*>(tile);

        if (!buildableTile->isOwned()) {
            // Unowned — offer to buy
            if (player->getCash() >= buildableTile->getPlotCost()) {
                savedTile = buildableTile;
                currentStep = GameStep::WaitingBuyDecision;
                emit buyDecisionNeeded(player, buildableTile, buildableTile->getPlotCost());
                return;
            }
            // Can't afford — go to auction
            savedTile = buildableTile;
            currentStep = GameStep::WaitingAuction;
            emit auctionNeeded(buildableTile, constant::auctionReservePrice, constant::auctionBidIncrement);
            return;
        }

        if (buildableTile->getOwner() != player) {
            // Pay rent to owner
            cashType rent = buildableTile->getRent();
            double dRentBonused = rent;
            for (auto& t : findOwnTiles(buildableTile->getOwner())) {
                if (t->getType() == Tile::buildable && static_cast<Buildable*>(t)->getColor() == buildableTile->getColor()) {
                    dRentBonused *= constant::sameColorBonusMultiplier;
                }
            }
            cashType rentBonused = static_cast<cashType>(dRentBonused / 100.0f) * 100;

            if (player->getCash() < rentBonused) {
                utils::Logger::getInstance().log("handleTileEvent(): Player cannot afford rent, needs to sell.");
                savedTile = buildableTile;
                savedAmount = rentBonused;
                currentStep = GameStep::WaitingSell;
                emit sellDecisionNeeded(player, rentBonused);
                return;
            }

            // Can afford rent
            player->addCash(-rentBonused);
            buildableTile->getOwner()->addCash(rentBonused);
            emit playerUpdated(player);
            emit playerUpdated(buildableTile->getOwner());
            emit rentPaid(player, buildableTile->getOwner(), buildableTile, rentBonused);
            emit boardUpdateNeeded();
        } else {
            // Own tile — offer to build
            if (buildableTile->getStatus() < Buildable::hotel && player->getCash() >= buildableTile->getHouseCost()) {
                int maxLevels = std::min(
                    static_cast<int>(Buildable::hotel - buildableTile->getStatus()),
                    static_cast<int>(player->getCash() / buildableTile->getHouseCost()));
                if (maxLevels > 0) {
                    savedTile = buildableTile;
                    currentStep = GameStep::WaitingBuild;
                    emit buildDecisionNeeded(player, buildableTile, buildableTile->getHouseCost(), maxLevels);
                    return;
                }
            }
        }
        break;
    }

    case Tile::prison:
        player->setPrisonTime(0);
        emit playerPrisoned(player, static_cast<Prison*>(tile));
        emit boardUpdateNeeded();
        break;

    case Tile::tax: {
        Tax* taxTile = static_cast<Tax*>(tile);
        cashType taxToPay = static_cast<cashType>(player->getCash() * taxTile->getTaxRate() / 100.0f) * 100;
        utils::Logger::getInstance().log("handleTileEvent(): Tax paid " + std::to_string(taxToPay));
        player->addCash(-taxToPay);
        emit playerUpdated(player);
        emit taxPaid(player, taxTile, taxToPay);
        emit boardUpdateNeeded();
        break;
    }

    case Tile::random: {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 5);
        int randomValue = distr(gen);
        utils::Logger::getInstance().log("handleTileEvent(): Random value=" + std::to_string(randomValue));

        switch (randomValue) {
        case 0: {
            Prison* pri = static_cast<Prison*>(tiles[findNextTile(Tile::prison, player->getPosition())]);
            if (pri != nullptr) {
                player->setPosition(findTile(pri));
                player->setPrisonTime(0);
                emit playerPrisoned(player, pri);
            }
            break;
        }
        case 1: {
            Tax* tax = static_cast<Tax*>(tiles[findNextTile(Tile::tax, player->getPosition())]);
            if (tax != nullptr) {
                cashType taxToPay = static_cast<cashType>(player->getCash() * tax->getTaxRate() / 100.0f) * 100;
                player->addCash(-taxToPay);
                emit playerUpdated(player);
                emit taxPaid(player, tax, taxToPay);
            }
            break;
        }
        case 2: {
            Home* home = static_cast<Home*>(tiles[findNextTile(Tile::home, player->getPosition())]);
            if (home != nullptr) {
                player->addCash(constant::homeReward);
                emit playerUpdated(player);
                emit homeRewardCollected(player, constant::homeReward);
            }
            break;
        }
        case 3: {
            auto ownTiles = findOwnTiles(player);
            if (!ownTiles.empty()) {
                int ran = std::uniform_int_distribution<>(0, ownTiles.size() - 1)(gen);
                Buildable* bt = static_cast<Buildable*>(ownTiles[ran]);
                if (bt->getStatus() < Buildable::hotel && player->getCash() >= bt->getHouseCost()) {
                    int maxLevels = std::min(
                        static_cast<int>(Buildable::hotel - bt->getStatus()),
                        static_cast<int>(player->getCash() / bt->getHouseCost()));
                    if (maxLevels > 0) {
                        savedTile = bt;
                        currentStep = GameStep::WaitingBuild;
                        emit buildDecisionNeeded(player, bt, bt->getHouseCost(), maxLevels);
                        return;
                    }
                }
            }
            break;
        }
        case 4: {
            auto ownTiles = findOwnTiles(player);
            if (!ownTiles.empty()) {
                int ran = std::uniform_int_distribution<>(0, ownTiles.size() - 1)(gen);
                Buildable* bt = static_cast<Buildable*>(ownTiles[ran]);
                bt->setOwner(nullptr);
                bt->setStatus(Buildable::empty);
                emit propertyChanged(bt);
                emit randomEventTriggered(bt, QString("Random destruction! %1 lost tile #%2.")
                    .arg(QString::fromStdString(player->getNickname()))
                    .arg(findTile(bt)));
            }
            break;
        }
        case 5: {
            cashType cur = player->getCash();
            player->setCash(cur * 2);
            emit playerUpdated(player);
            emit randomEventTriggered(tile, QString("Random earn! %1's money doubled to $%2.")
                .arg(QString::fromStdString(player->getNickname()))
                .arg(cur * 2));
            break;
        }
        }

        emit boardUpdateNeeded();
        break;
    }

    case Tile::home:
        // Nothing special — just landing on home
        break;

    default:
        break;
    }

    // If we reached here (no async wait happened), advance to next turn
    if (currentStep == GameStep::HandlingTile) {
        currentStep = GameStep::TurnEnd;
        advance();
    }
}
