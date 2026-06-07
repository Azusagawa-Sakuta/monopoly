#include "gamemainwidget.h"
#include "ui_gamemainwidget.h"
#include "auctionwidget.h"
#include "sellwidget.h"

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QPen>
#include <QBrush>
#include <QResizeEvent>
#include <QThread>
#include <cmath>
#include <vector>
#include <random>
#include <QGuiApplication>
#include <QTimer>
#include <QMovie>

#include "../backend/game.h"

gameMainWidget::gameMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameMainWidget),
    scene(new QGraphicsScene(this)),
    firstDiceNumber(0),
    isPrisonDiceMode(false) {

    for (auto& scenePlayer : scenePlayers) {
        scenePlayer = new QGraphicsScene(this);
    }

    ui->setupUi(this);
    showMaximized();

    for (auto& scenePlayer : scenePlayers) {
        scenePlayer->clear();
    }

    int playerNum = loadImage();
    ui->mapView->setScene(scene);

    ui->rollDiceButton->setDisabled(true);
    rollDiceBackdoor(1, 1);

    for (auto&& [playerAvatarGraphics, scenePlayer] : std::views::zip(
        ui->playerAvatarGraphicses,
        scenePlayers | std::views::as_const
    )) {
        playerAvatarGraphics->setScene(scenePlayer);
    }

    for (auto&& [playerAvatarGraphics, playerInfos, playerNickname] : std::views::zip(
        ui->playerAvatarGraphicses,
        ui->allPlayerInfos,
        ui->playerNicknames
    ) | std::views::drop(playerNum)) {
        playerAvatarGraphics->hide();
        for (auto& playerInfo : playerInfos) {
            playerInfo->hide();
        }
        playerNickname->hide();
    }

    // Connect backend signals to UI slots
    connectGameSignals();

    update();
    updatePlayerInfo();

    // Start the game loop
    auto& g = game::gamePlay::GameInstance::getInstance();
    g.advance();
}

gameMainWidget::~gameMainWidget() {
    delete ui;
}

void gameMainWidget::connectGameSignals() {
    auto& g = game::gamePlay::GameInstance::getInstance();

    connect(&g, &game::gamePlay::GameInstance::turnStarted, this, &gameMainWidget::onTurnStarted);
    connect(&g, &game::gamePlay::GameInstance::gameEnded, this, &gameMainWidget::onGameEnded);
    connect(&g, &game::gamePlay::GameInstance::diceRolled, this, &gameMainWidget::onDiceRolled);
    connect(&g, &game::gamePlay::GameInstance::playerMoved, this, &gameMainWidget::onPlayerMoved);
    connect(&g, &game::gamePlay::GameInstance::playerPassedGo, this, &gameMainWidget::onPlayerPassedGo);
    connect(&g, &game::gamePlay::GameInstance::buyDecisionNeeded, this, &gameMainWidget::onBuyDecisionNeeded);
    connect(&g, &game::gamePlay::GameInstance::auctionNeeded, this, &gameMainWidget::onAuctionNeeded);
    connect(&g, &game::gamePlay::GameInstance::buildDecisionNeeded, this, &gameMainWidget::onBuildDecisionNeeded);
    connect(&g, &game::gamePlay::GameInstance::sellDecisionNeeded, this, &gameMainWidget::onSellDecisionNeeded);
    connect(&g, &game::gamePlay::GameInstance::rentPaid, this, &gameMainWidget::onRentPaid);
    connect(&g, &game::gamePlay::GameInstance::taxPaid, this, &gameMainWidget::onTaxPaid);
    connect(&g, &game::gamePlay::GameInstance::homeRewardCollected, this, &gameMainWidget::onHomeRewardCollected);
    connect(&g, &game::gamePlay::GameInstance::playerPrisoned, this, &gameMainWidget::onPlayerPrisoned);
    connect(&g, &game::gamePlay::GameInstance::prisonDiceNeeded, this, &gameMainWidget::onPrisonDiceNeeded);
    connect(&g, &game::gamePlay::GameInstance::prisonPayOutNeeded, this, &gameMainWidget::onPrisonPayOutNeeded);
    connect(&g, &game::gamePlay::GameInstance::randomEventTriggered, this, &gameMainWidget::onRandomEventTriggered);
    connect(&g, &game::gamePlay::GameInstance::playerUpdated, this, &gameMainWidget::onPlayerUpdated);
    connect(&g, &game::gamePlay::GameInstance::playerBankrupted, this, &gameMainWidget::onPlayerBankrupted);
    connect(&g, &game::gamePlay::GameInstance::propertyChanged, this, &gameMainWidget::onPropertyChanged);
    connect(&g, &game::gamePlay::GameInstance::boardUpdateNeeded, this, &gameMainWidget::onBoardUpdateNeeded);
}

// ─── Signal Handlers ──────────────────────────────────────────────────────────

void gameMainWidget::onTurnStarted(game::player::Player* player) {
    Q_UNUSED(player);
    update();
}

void gameMainWidget::onGameEnded(game::player::Player* winner) {
    QMessageBox::information(this, "Game Over", QString::fromStdString(winner->getNickname()) + " Won!!");
    QApplication::quit();
}

void gameMainWidget::onDiceRolled(game::player::Player* player, int dice1, int dice2) {
    Q_UNUSED(dice1);
    Q_UNUSED(dice2);
    isPrisonDiceMode = false;

    if (player->isComputer()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 6);
        int d1 = distr(gen);
        int d2 = distr(gen);
        int result = 0;
        if (d1 == d2) {
            int d3 = distr(gen);
            result = (d1 == d3) ? (-3 * d3) : (d1 * 2 + d3);
        } else {
            result = d1 + d2;
        }
        QString msg = QString::fromStdString(player->getNickname())
                      + " rolled " + QString::number(result) + ".";
        QTimer::singleShot(200, this, [this, result, msg]() {
            QMessageBox::information(this, "Computer Roll Dice", msg);
            game::gamePlay::GameInstance::getInstance().provideInput(result);
        });
    } else {
        ui->rollDiceButton->setDisabled(false);
    }
}

void gameMainWidget::onPlayerMoved(game::player::Player* player, int fromPos, int toPos) {
    Q_UNUSED(player);
    Q_UNUSED(fromPos);
    Q_UNUSED(toPos);
    update();
}

void gameMainWidget::onPlayerPassedGo(game::player::Player* player, game::cashType reward) {
    QMessageBox::information(this, "Home Reward",
        QString::fromStdString(player->getNickname() + " received $" + std::to_string(reward) + " for passing the start tile."));
}

void gameMainWidget::onBuyDecisionNeeded(game::player::Player* player, game::gamePlay::Buildable* tile, game::cashType cost) {
    auto& g = game::gamePlay::GameInstance::getInstance();

    if (player->isComputer()) {
        QMessageBox::information(this, "Computer Buy",
            QString::fromStdString(player->getNickname()) + " chose to buy this tile.");
        g.provideInput(true);
    } else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Buy Property",
            QString::fromStdString("Do you want to buy tile #" + std::to_string(g.findTile(tile)) + " for $" + std::to_string(cost)),
            QMessageBox::Ok | QMessageBox::Cancel);
        g.provideInput(reply == QMessageBox::Ok);
    }
}

void gameMainWidget::onAuctionNeeded(game::gamePlay::Buildable* tile, game::cashType reservePrice, game::cashType bidIncrement) {
    static auctionWidget *m_auctionWidget = new auctionWidget(this);
    if (!m_auctionWidget->isVisible()) {
        auto f = Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint;
        f &= ~Qt::WindowCloseButtonHint;
        m_auctionWidget->setWindowFlags(f);
        m_auctionWidget->show();
        m_auctionWidget->initialize(tile, reservePrice, bidIncrement);
    }
}

void gameMainWidget::onBuildDecisionNeeded(game::player::Player* player, game::gamePlay::Buildable* tile, game::cashType houseCost, int maxLevels) {
    Q_UNUSED(houseCost);
    auto& g = game::gamePlay::GameInstance::getInstance();

    if (player->isComputer()) {
        game::gamePlay::Buildable::buildStatus current = tile->getStatus();
        std::random_device rd;
        std::mt19937 gen(rd());
        int build = std::uniform_int_distribution<>(static_cast<int>(current), maxLevels)(gen);
        QMessageBox::information(this, "Computer Building",
            QString::fromStdString(player->getNickname()) + " decided to build " + QString::number(build) + " more level(s).");
        g.provideInput(static_cast<game::gamePlay::Buildable::buildStatus>(build));
    } else {
        int level = QInputDialog::getInt(this, "Build House",
            QString::fromStdString("How many levels to build on tile #" + std::to_string(g.findTile(tile)) + "?"),
            0, 0, maxLevels);
        g.provideInput(static_cast<game::gamePlay::Buildable::buildStatus>(level + tile->getStatus()));
    }
}

void gameMainWidget::onSellDecisionNeeded(game::player::Player* player, game::cashType amountNeeded) {
    auto& g = game::gamePlay::GameInstance::getInstance();

    if (player->isComputer()) {
        auto ownTiles = g.findOwnTiles(player);
        game::cashType cash = player->getCash();
        game::cashType remaining = amountNeeded - cash;
        std::vector<game::gamePlay::Buildable*> toSell;
        for (const auto& tile : ownTiles) {
            if (remaining <= 0) break;
            auto buildable = static_cast<game::gamePlay::Buildable*>(tile);
            if (buildable->getValue() <= remaining) {
                toSell.push_back(buildable);
                remaining -= buildable->getValue();
            }
        }
        if (remaining > 0) {
            QMessageBox::information(this, "Computer Sell Properties",
                QString::fromStdString(player->getNickname()) + " couldn't afford the payment and went bankrupt.");
            g.provideInput(std::vector<game::gamePlay::Buildable*>());
        } else {
            QMessageBox::information(this, "Computer Sell Properties",
                QString::fromStdString(player->getNickname()) + " sold properties to afford the payment.");
            g.provideInput(toSell);
        }
    } else {
        static sellWidget *m_sellWidget = new sellWidget(this);
        if (!m_sellWidget->isVisible()) {
            auto f = Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint;
            f &= ~Qt::WindowCloseButtonHint;
            m_sellWidget->setWindowFlags(f);
            m_sellWidget->show();
            m_sellWidget->initialize(amountNeeded);
        }
    }
}

void gameMainWidget::onRentPaid(game::player::Player* payer, game::player::Player* owner, game::gamePlay::Buildable* tile, game::cashType amount) {
    auto& g = game::gamePlay::GameInstance::getInstance();
    QMessageBox::information(this, "Rent Paid",
        QString::fromStdString(payer->getNickname() + " paid $" + std::to_string(amount) +
            " on tile #" + std::to_string(g.findTile(tile)) + " to " + owner->getNickname()));
}

void gameMainWidget::onTaxPaid(game::player::Player* player, game::gamePlay::Tax* tile, game::cashType amount) {
    auto& g = game::gamePlay::GameInstance::getInstance();
    QMessageBox::information(this, "Tax Paid",
        QString::fromStdString(player->getNickname() + " paid $" + std::to_string(amount) +
            " for tax on tile #" + std::to_string(g.findTile(tile))));
}

void gameMainWidget::onHomeRewardCollected(game::player::Player* player, game::cashType amount) {
    QMessageBox::information(this, "Home Reward",
        QString::fromStdString(player->getNickname() + " received $" + std::to_string(amount) + " for passing the start tile."));
}

void gameMainWidget::onPlayerPrisoned(game::player::Player* player, game::gamePlay::Prison* tile) {
    Q_UNUSED(tile);
    QMessageBox::information(this, "Prisoned",
        QString::fromStdString(player->getNickname() + " was sent to prison."));
}

void gameMainWidget::onPrisonDiceNeeded(game::player::Player* player) {
    isPrisonDiceMode = true;

    if (player->isComputer()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 6);
        int d1 = distr(gen);
        int d2 = distr(gen);
        loadDice(d1, d2);

        bool escaped = (d1 == d2);
        int result = escaped ? (d1 + d2) : 0;
        QString msg = QString::fromStdString(player->getNickname())
                      + (escaped ? " rolled doubles (" + QString::number(result) + ") and escaped prison"
                                 : " rolled " + QString::number(d1 + d2) + ", not doubles");
        QTimer::singleShot(1800, this, [this, result, msg]() {
            QMessageBox::information(this, "Computer Roll Dice In Prison", msg);
            game::gamePlay::GameInstance::getInstance().provideInput(result);
        });
    } else {
        ui->rollDiceButton->setDisabled(false);
    }
}

void gameMainWidget::onPrisonPayOutNeeded(game::player::Player* player, game::cashType cost) {
    Q_UNUSED(player);
    Q_UNUSED(cost);
    // UI doesn't support prison payout — always decline
    auto& g = game::gamePlay::GameInstance::getInstance();
    g.provideInput(false);
}

void gameMainWidget::onRandomEventTriggered(game::gamePlay::Tile* tile, const QString& description) {
    Q_UNUSED(tile);
    QMessageBox::information(this, "Random Event", description);
}

void gameMainWidget::onPlayerUpdated(game::player::Player* player) {
    Q_UNUSED(player);
    updatePlayerInfo();
    update();
}

void gameMainWidget::onPlayerBankrupted(game::player::Player* player) {
    QMessageBox::information(this, "Bankrupt",
        QString::fromStdString(player->getNickname() + " went bankrupt!"));
    update();
}

void gameMainWidget::onPropertyChanged(game::gamePlay::Buildable* tile) {
    Q_UNUSED(tile);
    update();
}

void gameMainWidget::onBoardUpdateNeeded() {
    update();
    auto& g = game::gamePlay::GameInstance::getInstance();
    g.provideInput(std::monostate());
}

// ─── Dice Rolling ─────────────────────────────────────────────────────────────

void gameMainWidget::on_rollDiceButton_clicked() {
    ui->rollDiceButton->setDisabled(true);

    if (ui->rollDiceButton->text() == "Roll dice") {
        int d1, d2;
        rollDice(d1, d2);

        if (d1 != d2) {
            // Not doubles - normal sum or prison fail
            int result = isPrisonDiceMode ? 0 : (d1 + d2);
            QTimer::singleShot(1800, this, [this, result]() {
                game::gamePlay::GameInstance::getInstance().provideInput(result);
                ui->rollDiceButton->setText("Roll dice");
            });
        } else {
            if (isPrisonDiceMode) {
                int result = d1 + d2;
                QTimer::singleShot(1800, this, [this, result]() {
                    game::gamePlay::GameInstance::getInstance().provideInput(result);
                    ui->rollDiceButton->setText("Roll dice");
                });
            } else {
                // Normal mode: doubles → prepare for 3rd die
                firstDiceNumber = d1;
                QTimer::singleShot(1800, this, [this]() {
                    ui->rollDiceButton->setText("Roll dice again");
                    ui->rollDiceButton->setDisabled(false);
                });
            }
        }
    } else {
        // Third die roll after doubles
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 6);
        int d3 = distr(gen);
        QString gifPath3 = ":/resources/dice/dice" + QString::number(d3) + ".gif";
        QMovie* movie3 = new QMovie(gifPath3);
        ui->diceLabel_1->setMovie(movie3);
        movie3->start();

        int result = (firstDiceNumber == d3) ? (-3 * d3) : (2 * firstDiceNumber + d3);
        firstDiceNumber = 0;
        QTimer::singleShot(1800, this, [this, result]() {
            game::gamePlay::GameInstance::getInstance().provideInput(result);
            ui->rollDiceButton->setText("Roll dice");
        });
    }
}

void gameMainWidget::rollDice(int& d1, int& d2) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 6);
    d1 = distr(gen);
    d2 = distr(gen);
    loadDice(d1, d2);
}

void gameMainWidget::loadDice(int d1, int d2) {
    QString gifPath1 = ":/resources/dice/dice" + QString::number(d1) + ".gif";
    QString gifPath2 = ":/resources/dice/dice" + QString::number(d2) + ".gif";
    QMovie* movie1 = new QMovie(gifPath1);
    QMovie* movie2 = new QMovie(gifPath2);
    ui->diceLabel_1->setMovie(movie1);
    ui->diceLabel_2->setMovie(movie2);
    movie1->start();
    movie2->start();
}

void gameMainWidget::rollDiceBackdoor(int d1, int d2) {
    loadDice(d1, d2);
}

// ─── Board Rendering ──────────────────────────────────────────────────────────

void gameMainWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    QSize newSize = event->size();
    ui->mapView->setGeometry(0, 0, newSize.width(), newSize.height());
    ui->mainLayout->setGeometry(this->rect());
}

void gameMainWidget::update() {
    scene->clear();
    paintMap();
    updatePlayerInfo();

    auto& g = game::gamePlay::GameInstance::getInstance();
    const auto& player = g.findPlayerIndex(g.getCurrentPlayer());
    for (auto& playerAvatarGraphics : ui->playerAvatarGraphicses) {
        playerAvatarGraphics->setBackgroundBrush(Qt::transparent);
    }

    const std::array<const Qt::GlobalColor, 4> backgroundColor = {
        Qt::red,
        Qt::yellow,
        Qt::cyan,
        Qt::green
    };
    ui->playerAvatarGraphicses[player]->setBackgroundBrush(backgroundColor[player]);
}

void gameMainWidget::updatePlayerInfo() {
    auto& g = game::gamePlay::GameInstance::getInstance();
    int computers = 0, players = 0;
    const auto& playerList = g.getPlayers();
    for (const auto& it : playerList) {
        if (typeid(*it) == typeid(game::player::ComputerPlayer)) {
            it->setNickname("Computer " + std::to_string(++computers));
        } else {
            it->setNickname("Player " + std::to_string(++players));
        }

        QGraphicsColorizeEffect* grayscaleEffect = new QGraphicsColorizeEffect();
        grayscaleEffect->setColor(Qt::gray);
        grayscaleEffect->setStrength(1.0);

        bool isBankrupt = it->getCash() <= 0 && it->isBankrupted();
        const auto curPlayerIdx = computers + players - 1;
        ui->playerNicknames[curPlayerIdx]->setText(QString::fromStdString(it->getNickname()));
        const std::array<const QString, 4> colorStr = { "Red", "Yellow", "Blue", "Green" };
        ui->allPlayerInfos[curPlayerIdx][0]->setText(QString::fromStdString("Value: $") + QString::number(it->getCash()));
        ui->allPlayerInfos[curPlayerIdx][1]->hide();
        ui->allPlayerInfos[curPlayerIdx][2]->setText(QString("Color: ") + colorStr[curPlayerIdx]);
        if (isBankrupt) ui->playerAvatarGraphicses[curPlayerIdx]->setGraphicsEffect(grayscaleEffect);
        else ui->playerAvatarGraphicses[curPlayerIdx]->setGraphicsEffect(nullptr);
    }
}

QPixmap gameMainWidget::getTileImage(game::gamePlay::Tile* tile) {
    QPixmap tileImage;
    if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
        const game::gamePlay::Buildable* buildableTile = static_cast<const game::gamePlay::Buildable*>(tile);
        std::string colors[] = {":/resources/tile/red.png", ":/resources/tile/green.png", ":/resources/tile/blue.png", ":/resources/tile/yellow.png"};
        tileImage = QPixmap(QString::fromStdString(colors[buildableTile->getColor()]));
    } else if (tile->getType() == game::gamePlay::Tile::TileType::home) {
        tileImage = QPixmap(":/resources/tile/go.png");
    } else if (tile->getType() == game::gamePlay::Tile::TileType::random) {
        tileImage = QPixmap(":/resources/tile/casino.png");
    } else if (tile->getType() == game::gamePlay::Tile::TileType::tax) {
        tileImage = QPixmap(":/resources/tile/ccf.png");
    } else if (tile->getType() == game::gamePlay::Tile::TileType::prison) {
        tileImage = QPixmap(":/resources/tile/prison.png");
    }
    return tileImage;
}

QPixmap gameMainWidget::getHouseImage(game::gamePlay::Tile* tile) {
    const game::gamePlay::Buildable* buildableTile = static_cast<const game::gamePlay::Buildable*>(tile);
    QPixmap tileImage = QPixmap(QString(":/resources/houses/p%1house%2.png")
        .arg(game::gamePlay::GameInstance::getInstance().findPlayerIndex(buildableTile->getOwner()) + 1)
        .arg(buildableTile->getStatus()));
    return tileImage;
}

QPixmap gameMainWidget::getPlayerIndicator(game::player::Player* p) {
    std::string imgs[] = {":/resources/symbol/1P.png", ":/resources/symbol/2P.png", ":/resources/symbol/3P.png", ":/resources/symbol/4P.png"};
    return QPixmap(QString::fromStdString(imgs[game::gamePlay::GameInstance::getInstance().findPlayerIndex(p)]));
}

void gameMainWidget::paintTile(int x, int y, int index, int depth, game::gamePlay::Tile* tile) {
    auto tileImage = getTileImage(tile);

    int offsetX = (-tileImage.width()) / 2;
    int offsetY = (-tileImage.height()) / 2;

    auto pixmapItem = scene->addPixmap(tileImage);
    pixmapItem->setPos(x + offsetX, y + offsetY);
    pixmapItem->setZValue(depth);
    scene->addText(QString::number(index))->setPos(x, y);

    if (tile->getType() == game::gamePlay::Tile::TileType::buildable && static_cast<game::gamePlay::Buildable*>(tile)->isOwned())
        paintHouse(x, y, depth, getHouseImage(tile));

    int i = 0;
    auto& players = game::gamePlay::GameInstance::getInstance().getPlayers();
    for (auto& a : players) {
        if (!a->isBankrupted() && a->getPosition() == index) {
            QPixmap playerImage(getPlayerIndicator(a));
            auto item = scene->addPixmap(playerImage);
            item->setPos(x + offsetX, y + offsetY - i * 64);
            item->setZValue(100);
            i++;
        }
    }
}

void gameMainWidget::paintHouse(int x, int y, int depth, QPixmap tileImage) {
    int offsetX = (-tileImage.width()) / 2;
    int offsetY = (-tileImage.height()) / 2;

    auto pixmapItem = scene->addPixmap(tileImage);
    pixmapItem->setPos(x + offsetX, y + offsetY);
    pixmapItem->setZValue(depth);
}

void gameMainWidget::paintMap() {
    int spacing = 5;
    int horizontalSpacing = 64 + spacing * 2;
    int verticalSpacing = 32 + spacing;

    const std::vector<game::gamePlay::Tile*>& tiles = game::gamePlay::GameInstance::getInstance().getTiles();

    int numTiles = tiles.size();
    int numCols = static_cast<int>(std::ceil(numTiles / 4.0f)) + 1;
    int numRows = static_cast<int>(std::ceil(numTiles / 2.0f)) - numCols + 2;

    int index = 0;
    int depth = 0;

    // Left column (bottom to top)
    for (int row = numRows - 1; row >= 0 && index < numTiles; --row) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = horizontalSpacing * (-numRows + row + 1);
        int y = row * verticalSpacing;
        paintTile(x, y, index++, depth--, tile);
    }

    // Top row (left to right)
    for (int col = 1; col < numCols && index < numTiles; ++col) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = horizontalSpacing * (-numRows + col + 1);
        int y = -col * verticalSpacing;
        paintTile(x, y, index++, depth--, tile);
    }

    depth += 2;

    // Right column (top to bottom)
    for (int row = 1; row < numRows && index < numTiles; ++row) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = horizontalSpacing * row;
        int y = (-numRows + row + 1) * verticalSpacing;
        paintTile(x, y, index++, depth++, tile);
    }

    // Bottom row (right to left)
    for (int col = numCols - 2; col > 0 && index < numTiles; --col) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = horizontalSpacing * col;
        int y = (numCols - col - 1) * verticalSpacing;
        paintTile(x, y, index++, depth++, tile);
    }
}

int gameMainWidget::loadImage() {
    auto& g = game::gamePlay::GameInstance::getInstance();
    auto playerList = g.getPlayers();
    for (auto&& [it, scenePlayer, playerAvatarGraphics] : std::views::zip(
        playerList | std::views::as_const,
        scenePlayers | std::views::as_const,
        ui->playerAvatarGraphicses | std::views::as_const
    )) {
        const QPixmap pixmap(QString::fromStdString(it->getImagePath()));
        const QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(scaledPixmap);
        scenePlayer->addItem(item);
        item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
        playerAvatarGraphics->fitInView(scenePlayer->sceneRect(), Qt::KeepAspectRatio);
    }
    return playerList.size();
}
