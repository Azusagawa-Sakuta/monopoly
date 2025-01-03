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
#include <cmath>
#include <vector>
#include <variant>
#include <QGuiApplication>
#include <QTimer>
#include <QMovie>
#include <QThread>
#include "../backend/game.h"

gameMainWidget::gameMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameMainWidget),
    scene(new QGraphicsScene(this)),
    scenePlayer1(new QGraphicsScene(this)),
    scenePlayer2(new QGraphicsScene(this)),
    scenePlayer3(new QGraphicsScene(this)),
    scenePlayer4(new QGraphicsScene(this)),
    timer(new QTimer(this)) {
    ui->setupUi(this);
    showMaximized();
    //initializeGameInstance();
    scenePlayer1->clear();
    scenePlayer2->clear();
    scenePlayer3->clear();
    scenePlayer4->clear();
    int playerNum = loadImage();
    ui->mapView->setScene(scene);

    ui->rollDiceButton->setDisabled(true);

    ui->playerAvatarGraphics_1->setScene(scenePlayer1);
    ui->playerAvatarGraphics_2->setScene(scenePlayer2);
    ui->playerAvatarGraphics_3->setScene(scenePlayer3);
    ui->playerAvatarGraphics_4->setScene(scenePlayer4);
    switch(playerNum) {
    case 2:
        ui->playerAvatarGraphics_3->hide();
        ui->playerInfo_3_1->hide();
        ui->playerInfo_3_2->hide();
        ui->playerInfo_3_3->hide();
        ui->playerNickname_3->hide();
        [[fallthrough]];
    case 3:
        ui->playerAvatarGraphics_4->hide();
        ui->playerInfo_4_1->hide();
        ui->playerInfo_4_2->hide();
        ui->playerInfo_4_3->hide();
        ui->playerNickname_4->hide();
    }
    auto& g = game::gamePlay::GameInstance::getInstance();
    // Connect the timer's timeout signal to the onTick slot
    connect(timer, &QTimer::timeout, this, &gameMainWidget::onTick);

    qDebug() << "1";
    timer->start(100);
    update();
    g.notifyUserInput(std::monostate());
}

gameMainWidget::~gameMainWidget()
{
    delete ui;
}

void gameMainWidget::onTick() {
    auto& g = game::gamePlay::GameInstance::getInstance();
    auto e = g.getActiveEvent();
    if (e != game::gamePlay::GameInstance::eventType::None)
        update();
    switch (e) {
    case game::gamePlay::GameInstance::eventType::None:
        break;
    case game::gamePlay::GameInstance::eventType::Update:
        qDebug() << "Update!!";
        update();
        g.notifyUserInput(std::monostate());
        break;
    case game::gamePlay::GameInstance::eventType::Dice: {
        ui->rollDiceButton->setDisabled(false);
        break;
    }
    case game::gamePlay::GameInstance::eventType::Prisoned: {
        QMessageBox::information(this, "Prisoned", QString::fromStdString("Player #" + std::to_string(g.findPlayerPos(g.getCurrentPlayer()) + 1) + " are prisoned."));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::PrisonPayOut: {
        //QMessageBox::information(this, "Prison Payout", "You can pay to get out but we didn't implement this feature.");
        g.notifyUserInput(false);
        break;
    }
    case game::gamePlay::GameInstance::eventType::PrisonDice: {
        ui->rollDiceButton->setDisabled(false);
        break;
    }
    case game::gamePlay::GameInstance::eventType::Buy: {
        game::gamePlay::GameInstance::buyRequest req = std::any_cast<game::gamePlay::GameInstance::buyRequest>(g.getActiveEventParam());
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Buy Property", QString::fromStdString("Do you want to buy tile #" + std::to_string(g.findTile(req.tile)) + " for $" + std::to_string(req.price)), QMessageBox::Ok | QMessageBox::Cancel);
        if (reply == QMessageBox::Ok) {
            g.notifyUserInput(true);
        } else {
            g.notifyUserInput(false);
        }
        break;
    }
    case game::gamePlay::GameInstance::eventType::RentPaid: {
        game::gamePlay::GameInstance::rentRequest req = std::any_cast<game::gamePlay::GameInstance::rentRequest>(g.getActiveEventParam());
        QMessageBox::information(this, "Rent Paid", QString::fromStdString("Player #" + std::to_string(g.findPlayerPos(g.getCurrentPlayer()) + 1) + " paid $" + std::to_string(req.rent) + " on tile #" + std::to_string(g.findTile(req.tile)) + " to player #" + std::to_string(g.findPlayerPos(req.tile->getOwner()) + 1)));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::Taxed: {
        game::gamePlay::GameInstance::taxRequest req = std::any_cast<game::gamePlay::GameInstance::taxRequest>(g.getActiveEventParam());
        QMessageBox::information(this, "Tax Paid", QString::fromStdString("Player #" + std::to_string(g.findPlayerPos(g.getCurrentPlayer()) + 1) + " paid $" + std::to_string(req.tax) + " for tax on tile #" + std::to_string(g.findTile(req.tile))));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::HomeReward: {
        game::cashType req = std::any_cast<game::cashType>(g.getActiveEventParam());
        QMessageBox::information(this, "Home Reward", QString::fromStdString("Player #" + std::to_string(g.findPlayerPos(g.getCurrentPlayer()) + 1) + " received $" + std::to_string(req) + " for passing the start tile"));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::Auction: {
        static auctionWidget *m_auctionWidget = new auctionWidget(this);
        if (!m_auctionWidget) {
            qDebug() << "Creating new auction widget..";
            m_auctionWidget = new auctionWidget(this);
        }
        if (!m_auctionWidget->isVisible()) {
            auto f = Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint;
            f &= ~Qt::WindowCloseButtonHint;
            m_auctionWidget->setWindowFlags(f);
            m_auctionWidget->show();
            m_auctionWidget->initialize();
        }
        break;
    }
    case game::gamePlay::GameInstance::eventType::Build: {
        game::gamePlay::Buildable* tile = std::any_cast<game::gamePlay::Buildable*>(g.getActiveEventParam());
        game::gamePlay::Buildable::buildStatus current = tile->getStatus();
        int max = std::min(static_cast<long long>(game::gamePlay::Buildable::buildStatus::hotel - current), g.getCurrentPlayer()->getCash() / tile->getHouseCost());
        if (max) 
            g.notifyUserInput(static_cast<game::gamePlay::Buildable::buildStatus>(QInputDialog::getInt(this, "Build House", QString::fromStdString("How many houses do you want to build on tile #" + std::to_string(g.findTile(tile)) + "?"), 0, 0, max)));
        else {
            QMessageBox::information(this, "Build Fail", "You do not have enough money to build houses or hotel");
        }
        break;
    }
    case game::gamePlay::GameInstance::eventType::RandomDestruction: {
        game::gamePlay::Buildable* tile = std::any_cast<game::gamePlay::Buildable*>(g.getActiveEventParam());
        QMessageBox::information(this, "Random Destruction", QString::fromStdString("Player #" + std::to_string(g.findPlayerPos(g.getCurrentPlayer()) + 1) + "'s tile #" + std::to_string(g.findTile(tile)) + " was destroyed... sad :("));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::RandomEarn: {
        game::cashType req = std::any_cast<game::cashType>(g.getActiveEventParam());
        QMessageBox::information(this, "Random Earn", QString::fromStdString("Player #" + std::to_string(g.findPlayerPos(g.getCurrentPlayer()) + 1) + " earned $" + std::to_string(req) + " in random event! Congrats!"));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::Sell: {
        auto ownTiles = g.findOwnTiles(g.getCurrentPlayer());
        static sellWidget *m_sellWidget = new sellWidget(this);
        if (!m_sellWidget) {
            qDebug() << "Creating new sell widget..";
            m_sellWidget = new sellWidget(this);
        }
        if (!m_sellWidget->isVisible()) {
            auto f = Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint;
            f &= ~Qt::WindowCloseButtonHint;
            m_sellWidget->setWindowFlags(f);
            m_sellWidget->show();
            m_sellWidget->initialize();
        }
        break;
    }
    }
}

void gameMainWidget::initializeGameInstance() {
    auto& g = game::gamePlay::GameInstance::getInstance();
    g.callbackPlayerUpdate = [this](game::player::Player* p) {
        if (p->getPosition() == 0) {
            ui->playerInfo_1_1->setText(QString::fromStdString("Value: $" + std::to_string(p->getCash())));
        }
        else if (p->getPosition() == 1) {
            ui->playerInfo_2_1->setText(QString::fromStdString("Value: $" + std::to_string(p->getCash())));
        }
        else if (p->getPosition() == 2) {
            ui->playerInfo_3_1->setText(QString::fromStdString("Value: $" + std::to_string(p->getCash())));
        }
        else if (p->getPosition() == 3) {
            ui->playerInfo_4_1->setText(QString::fromStdString("Value: $" + std::to_string(p->getCash())));
        }
        update();
    };
    g.callbackDice = [this](game::player::Player* p, int d1, int d2) {
        QMessageBox::information(this, "Dice Roll", QString::fromStdString(p->getNickname() + " rolled " + std::to_string(d1) + " and " + std::to_string(d2)));
    };
}

void gameMainWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    QSize newSize = event->size();
    // 更新 QGraphicsView 的大小以填充窗口
    ui->mapView->setGeometry(0, 0, newSize.width(), newSize.height());
    ui->mainLayout->setGeometry(this->rect());
    //update();
}

void gameMainWidget::update() {
    scene->clear(); // Clear old items
    paintMap();
    updatePlayerInfo();
    switch (game::gamePlay::GameInstance::getInstance().findPlayerPos(game::gamePlay::GameInstance::getInstance().getCurrentPlayer())) {
    case 0:
        ui->playerAvatarGraphics_1->setBackgroundBrush(Qt::red);
        ui->playerAvatarGraphics_2->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_3->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_4->setBackgroundBrush(Qt::transparent);
        break;
    case 1:
        ui->playerAvatarGraphics_1->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_2->setBackgroundBrush(Qt::cyan);
        ui->playerAvatarGraphics_3->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_4->setBackgroundBrush(Qt::transparent);
        break;
    case 2:
        ui->playerAvatarGraphics_1->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_2->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_3->setBackgroundBrush(Qt::yellow);
        ui->playerAvatarGraphics_4->setBackgroundBrush(Qt::transparent);
        break;
    case 3:
        ui->playerAvatarGraphics_1->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_2->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_3->setBackgroundBrush(Qt::transparent);
        ui->playerAvatarGraphics_4->setBackgroundBrush(Qt::green);
        break;
    }
    qDebug() << "Updated";
}

void gameMainWidget::updatePlayerInfo() {
    int computers = 0, players = 0;
    const auto& playerList = game::gamePlay::GameInstance::getInstance().getPlayers();
    for (const auto& it : playerList) {
        if (typeid(*it) == typeid(game::player::ComputerPlayer)) {
            it->setNickname("Computer" + std::to_string(++computers));
        }
        else {
            it->setNickname("Player" + std::to_string(++players));
        }

        // Create a grayscale effect
        QGraphicsColorizeEffect* grayscaleEffect = new QGraphicsColorizeEffect();
        grayscaleEffect->setColor(Qt::gray);
        grayscaleEffect->setStrength(1.0);

        bool isBankrupt = it->getCash() == 0 && it->isBankrupted();
        if (computers + players == 1) {
            ui->playerNickname_1->setText(QString::fromStdString(it->getNickname()));
            ui->playerInfo_1_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            ui->playerInfo_1_2->hide();
            ui->playerInfo_1_3->setText(QString::fromStdString("Color: " "Red"));
            if (isBankrupt) {
                ui->playerAvatarGraphics_1->setGraphicsEffect(grayscaleEffect);
            } else {
                ui->playerAvatarGraphics_1->setGraphicsEffect(nullptr);
            }
        }
        else if (computers + players == 2) {
            ui->playerNickname_2->setText(QString::fromStdString(it->getNickname()));
            ui->playerInfo_2_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            ui->playerInfo_2_2->hide();
            ui->playerInfo_2_3->setText(QString::fromStdString("Color: " "Cyan"));
            if (isBankrupt) {
                ui->playerAvatarGraphics_2->setGraphicsEffect(grayscaleEffect);
            } else {
                ui->playerAvatarGraphics_2->setGraphicsEffect(nullptr);
            }
        }
        else if (computers + players == 3) {
            ui->playerNickname_3->setText(QString::fromStdString(it->getNickname()));
            ui->playerInfo_3_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            ui->playerInfo_3_2->hide();
            ui->playerInfo_3_3->setText(QString::fromStdString("Color: " "Yellow"));
            if (isBankrupt) {
                ui->playerAvatarGraphics_3->setGraphicsEffect(grayscaleEffect);
            } else {
                ui->playerAvatarGraphics_3->setGraphicsEffect(nullptr);
            }
        }
        else if (computers + players == 4) {
            ui->playerNickname_4->setText(QString::fromStdString(it->getNickname()));
            ui->playerInfo_4_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            ui->playerInfo_4_2->hide();
            ui->playerInfo_4_3->setText(QString::fromStdString("Color: " "Green"));
            if (isBankrupt) {
                ui->playerAvatarGraphics_4->setGraphicsEffect(grayscaleEffect);
            } else {
                ui->playerAvatarGraphics_4->setGraphicsEffect(nullptr);
            }
        }
    }
}

QPixmap gameMainWidget::getTileImage(game::gamePlay::Tile* tile)
{
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
        tileImage = QPixmap(":/resources/tile/taxBureau.png");
    } else if (tile->getType() == game::gamePlay::Tile::TileType::prison) {
        tileImage = QPixmap(":/resources/tile/prison.png");
    }
    if (tileImage.isNull()) {
        qDebug() << "Failed to load image";
    }
    return tileImage;
}

QPixmap gameMainWidget::getHouseImage(game::gamePlay::Tile* tile)
{
    QPixmap tileImage;
    const game::gamePlay::Buildable* buildableTile = static_cast<const game::gamePlay::Buildable*>(tile);
    std::string colors[] = {":/resources/houses/house0.png", ":/resources/houses/house1.png", ":/resources/houses/house2.png", ":/resources/houses/house3.png", ":/resources/houses/house4.png", ":/resources/houses/house5.png", ":/resources/houses/house6.png"};
    tileImage = QPixmap(QString::fromStdString(colors[buildableTile->getStatus()]));
    if (tileImage.isNull()) {
        qDebug() << "Failed to load image";
    }
    return tileImage;
}

QPixmap gameMainWidget::getPlayerIndicator(game::player::Player* p)
{
    QPixmap img;
    std::string imgs[] = {":/resources/symbol/1P.png", ":/resources/symbol/2P.png", ":/resources/symbol/3P.png", ":/resources/symbol/4P.png"};
    img = QPixmap(QString::fromStdString(imgs[game::gamePlay::GameInstance::getInstance().findPlayerPos(p)]));
    if (img.isNull()) {
        qDebug() << "Failed to load image";
    }
    return img;
}

void gameMainWidget::paintTile(int x, int y, int index, int depth, game::gamePlay::Tile* tile) {
    auto tileImage = getTileImage(tile);
    
    int offsetX = (- tileImage.width()) / 2;
    int offsetY = (- tileImage.height()) / 2;

    bool paintTileIndex = true;

    auto pixmapItem = scene->addPixmap(tileImage);
    pixmapItem->setPos(x + offsetX, y + offsetY);
    pixmapItem->setZValue(depth);
    if (paintTileIndex) 
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
    int offsetX = (- tileImage.width()) / 2;
    int offsetY = (- tileImage.height()) / 2;

    auto pixmapItem = scene->addPixmap(tileImage);
    pixmapItem->setPos(x + offsetX, y + offsetY);
    pixmapItem->setZValue(depth);
}

void gameMainWidget::paintMap() {
    
    int spacing = 5;
    int horizontalSpacing = 64 + spacing * 2;
    int verticalSpacing = 32 + spacing;

    // Retrieve tiles from GameInstance
    const std::vector<game::gamePlay::Tile*>& tiles = game::gamePlay::GameInstance::getInstance().getTiles();

    // Define colors for the tiles
    //QColor colors[] = { Qt::red, Qt::green, Qt::blue, Qt::yellow };

    // Calculate the number of rows and columns to form a diamond shape
    int numTiles = tiles.size();
    int numCols = static_cast<int>(std::ceil(numTiles / 4.0f)) + 1;
    int numRows = static_cast<int>(std::ceil(numTiles / 2.0f)) - numCols + 2;

    int index = 0;

    int depth = 0;

    //scene->addRect(- tileW / 2.0f, - tileH / 2.0f, tileW, tileH, QPen(Qt::black), QBrush(Qt::blue));

    // Left column (bottom to top)
    for (int row = numRows - 1; row >= 0 && index < numTiles; --row) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = horizontalSpacing * (- numRows + row + 1);
        int y = row * verticalSpacing;
        paintTile(x, y, index++, depth--, tile);
    }

    // Top row (left to right)
    for (int col = 1; col < numCols && index < numTiles; ++col) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = horizontalSpacing * (- numRows + col + 1);
        int y = - col * verticalSpacing;
        paintTile(x, y, index++, depth--, tile);
    }

    depth += 2;

    // Right column (top to bottom)
    for (int row = 1; row < numRows && index < numTiles; ++row) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = horizontalSpacing * row;
        int y = (- numRows + row + 1) * verticalSpacing;
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
    int i = 0;
    for (const auto& it : playerList) {
        i++;
        QPixmap pixmap(QString::fromStdString(it->getImagePath()));
        QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(scaledPixmap);
        switch(i) {
        case 1:
            scenePlayer1->addItem(item);
            item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
            ui->playerAvatarGraphics_1->fitInView(scenePlayer1->sceneRect(), Qt::KeepAspectRatio);
            break;
        case 2:
            scenePlayer2->addItem(item);
            item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
            ui->playerAvatarGraphics_2->fitInView(scenePlayer2->sceneRect(), Qt::KeepAspectRatio);
            break;
        case 3:
            scenePlayer3->addItem(item);
            item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
            ui->playerAvatarGraphics_3->fitInView(scenePlayer3->sceneRect(), Qt::KeepAspectRatio);
            break;
        case 4:
            scenePlayer4->addItem(item);
            item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
            ui->playerAvatarGraphics_4->fitInView(scenePlayer4->sceneRect(), Qt::KeepAspectRatio);
        }
    }
    return i;
}
int firstDiceNumber;
void gameMainWidget::on_rollDiceButton_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    auto e = g.getActiveEvent();

    QStringList numberList = { "1", "2", "3", "4", "5", "6" };
    srand(time(nullptr));

    if(e == game::gamePlay::GameInstance::eventType::Dice) {
        int d1, d2, d3;
        if (ui->rollDiceButton->text() == "Roll dice") {
            rollDice(d1, d2);
            if (d1 != d2) {
                g.notifyUserInput(d1 + d2);
                ui->rollDiceButton->setDisabled(true);
            } else {
                firstDiceNumber = d1;
                ui->rollDiceButton->setText("Roll dice again");
            }
        }
        else {
            d3 = rand() % 6 + 1;
            QString gifPath3 = ":/resources/dice/dice" + numberList[d3 - 1] + ".gif";
            QMovie* movie3 = new QMovie(gifPath3);
            ui->diceLabel_1->setMovie(movie3);
            movie3->start();
            if (firstDiceNumber == d3) {
                g.notifyUserInput(-3 * d3);
                ui->rollDiceButton->setDisabled(true);
                ui->rollDiceButton->setText("Roll dice");
                firstDiceNumber = 0;
            } else {
                g.notifyUserInput(2 * firstDiceNumber + d3);
                ui->rollDiceButton->setDisabled(true);
                ui->rollDiceButton->setText("Roll dice");
                firstDiceNumber = 0;
            }
        }
    }
    else {
        int d1, d2;
        rollDice(d1, d2);
        if (d1 != d2) {
            rollDice(d1, d2);
            if (d1 != d2) {
                rollDice(d1, d2);
                if (d1 != d2) {
                    g.notifyUserInput(0);
                } else {
                    g.notifyUserInput(d1 + d2);
                }
            } else {
                g.notifyUserInput(d1 + d2);
            }
        } else {
            g.notifyUserInput(d1 + d2);
        }
    }
}

void gameMainWidget::rollDice(int& d1, int& d2) {
    d1 = rand() % 6 + 1;
    d2 = rand() % 6 + 1;
    QString gifPath1 = ":/resources/dice/dice" + QString::number(d1) + ".gif";
    QString gifPath2 = ":/resources/dice/dice" + QString::number(d2) + ".gif";
    QMovie* movie1 = new QMovie(gifPath1);
    QMovie* movie2 = new QMovie(gifPath2);
    ui->diceLabel_1->setMovie(movie1);
    ui->diceLabel_2->setMovie(movie2);
    movie1->start();
    movie2->start();
}

