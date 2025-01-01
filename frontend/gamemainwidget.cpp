#include "gamemainwidget.h"
#include "ui_gamemainwidget.h"
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
#include "../backend/game.h"

int d1, d2, d3;

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

    ui->diceLabel_1->hide();
    ui->diceLabel_2->hide();
    ui->rollDiceButton->hide();

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
    switch (e) {
    case game::gamePlay::GameInstance::eventType::None:
        break;
    case game::gamePlay::GameInstance::eventType::Update:
        qDebug() << "Update!!";
        update();
        g.notifyUserInput(std::monostate());
        break;
    case game::gamePlay::GameInstance::eventType::Dice: {
        d1 = d2 = d3 = 0;
        ui->diceLabel_1->show();
        ui->diceLabel_2->show();
        ui->rollDiceButton->show();
        break;
    }
    case game::gamePlay::GameInstance::eventType::Prisoned: {
        QMessageBox::information(this, "Prisoned", "You are prisoned for 3 same dice");
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::PrisonPayOut: {
        QMessageBox::information(this, "Prison Payout", "You can pay to get out but we didn't implement this feature.");
        g.notifyUserInput(false);
        break;
    }
    case game::gamePlay::GameInstance::eventType::PrisonDice: {
        d1 = d2 = d3 = 0;
        ui->diceLabel_1->show();
        ui->diceLabel_2->show();
        ui->rollDiceButton->show();
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
        QMessageBox::information(this, "Rent Paid", QString::fromStdString("You paid $" + std::to_string(req.rent) + " to the owner of tile #" + std::to_string(g.findTile(req.tile))));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::Taxed: {
        game::gamePlay::GameInstance::taxRequest req = std::any_cast<game::gamePlay::GameInstance::taxRequest>(g.getActiveEventParam());
        QMessageBox::information(this, "Tax Paid", QString::fromStdString("You paid $" + std::to_string(req.tax) + " for tax on tile #" + std::to_string(g.findTile(req.tile))));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::HomeReward: {
        game::cashType req = std::any_cast<game::cashType>(g.getActiveEventParam());
        QMessageBox::information(this, "Home Reward", QString::fromStdString("You received $" + std::to_string(req) + " for passing the start tile"));
        g.notifyUserInput(std::monostate());
        break;
    }
    case game::gamePlay::GameInstance::eventType::Auction: {
        game::gamePlay::GameInstance::auctionRequest req = std::any_cast<game::gamePlay::GameInstance::auctionRequest>(g.getActiveEventParam());
        QMessageBox::information(this, "Auction", QString::fromStdString("To be auctioned"));
        g.notifyUserInput(game::gamePlay::GameInstance::auctionResult{0, nullptr});
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
        if (computers + players == 1) {
            ui->playerNickname_1->setText(QString::fromStdString(it->getNickname()));
            ui->playerInfo_1_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            ui->playerInfo_1_2->hide();
            ui->playerInfo_1_3->setText(QString::fromStdString("Color: " "Red"));
        }
        else if (computers + players == 2) {
            ui->playerNickname_2->setText(QString::fromStdString(it->getNickname()));
            ui->playerInfo_2_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            ui->playerInfo_2_2->hide();
            ui->playerInfo_2_3->setText(QString::fromStdString("Color: " "Cyan"));
        }
        else if (computers + players == 3) {
            ui->playerNickname_3->setText(QString::fromStdString(it->getNickname()));
            ui->playerInfo_3_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            ui->playerInfo_3_2->hide();
            ui->playerInfo_3_3->setText(QString::fromStdString("Color: " "Yellow"));
        }
        else if (computers + players == 4) {
            ui->playerNickname_4->setText(QString::fromStdString(it->getNickname()));
            ui->playerInfo_4_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            ui->playerInfo_4_2->hide();
            ui->playerInfo_4_3->setText(QString::fromStdString("Color: " "Green"));
        }
    }
}

QPixmap gameMainWidget::getTileImage(game::gamePlay::Tile* tile)
{
    QPixmap tileImage;
    if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
        const game::gamePlay::Buildable* buildableTile = static_cast<const game::gamePlay::Buildable*>(tile);
        std::string colors[] = {":/resources/draft/tileRed.png", ":/resources/draft/tileGreen.png", ":/resources/draft/tileBlue.png", ":/resources/draft/tileYellow.png"};
        tileImage = QPixmap(QString::fromStdString(colors[buildableTile->getColor()]));
    } else {
        tileImage = QPixmap(":/resources/draft/tileTemplate.png");
    }
    if (tileImage.isNull()) {
        qDebug() << "Failed to load image";
    }
    return tileImage;
}

QPixmap gameMainWidget::getPlayerIndicator(game::player::Player* p)
{
    QPixmap img;
    std::string imgs[] = {":/resources/draft/1P.png", ":/resources/draft/2P.png", ":/resources/draft/3P.png", ":/resources/draft/4P.png"};
    img = QPixmap(QString::fromStdString(imgs[game::gamePlay::GameInstance::getInstance().findPlayerPos(p)]));
    if (img.isNull()) {
        qDebug() << "Failed to load image";
    }
    return img;
}

void gameMainWidget::paintTile(int x, int y, int index, int depth, QPixmap tileImage) {
    int offsetX = (- tileImage.width()) / 2;
    int offsetY = (- tileImage.height()) / 2;

    bool paintTileIndex = true;

    auto pixmapItem = scene->addPixmap(tileImage);
    pixmapItem->setPos(x + offsetX, y + offsetY);
    pixmapItem->setZValue(depth--);
    if (paintTileIndex) 
        scene->addText(QString::number(index))->setPos(x, y);
    
    int i = 0;
    auto& players = game::gamePlay::GameInstance::getInstance().getPlayers();
    for (auto& a : players) {
        if (a->getPosition() == index - 1) {
            QPixmap playerImage(getPlayerIndicator(a));
            auto item = scene->addPixmap(playerImage);
            item->setPos(x + offsetX, y + offsetY - i * 64);
            item->setZValue(100);
            i++;
        }
    }
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
        game::gamePlay::Tile* tile = tiles[index++];
        int x = horizontalSpacing * (- numRows + row + 1);
        int y = row * verticalSpacing;
        paintTile(x, y, index, depth--, getTileImage(tile));
    }

    // Top row (left to right)
    for (int col = 1; col < numCols && index < numTiles; ++col) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = horizontalSpacing * (- numRows + col + 1);
        int y = - col * verticalSpacing;
        paintTile(x, y, index, depth--, getTileImage(tile));
    }

    depth += 2;

    // Right column (top to bottom)
    for (int row = 1; row < numRows && index < numTiles; ++row) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = horizontalSpacing * row;
        int y = (- numRows + row + 1) * verticalSpacing;
        paintTile(x, y, index, depth++, getTileImage(tile));
    }

    // Bottom row (right to left)
    for (int col = numCols - 2; col > 0 && index < numTiles; --col) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = horizontalSpacing * col;
        int y = (numCols - col - 1) * verticalSpacing;
        paintTile(x, y, index, depth++, getTileImage(tile));
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

void gameMainWidget::on_rollDiceButton_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    auto e = g.getActiveEvent();

    QStringList numberList = { "1", "2", "3", "4", "5", "6" };
    srand(time(nullptr));
    if (ui->rollDiceButton->text() == "Roll dice") {
        d1 = rand() % 6 + 1;
        d2 = rand() % 6 + 1;
        QString gifPath1 = ":/resources/dice" + numberList[d1 - 1] + ".gif";
        QString gifPath2 = ":/resources/dice" + numberList[d2 - 2] + ".gif";
        QMovie* movie1 = new QMovie(gifPath1);
        QMovie* movie2 = new QMovie(gifPath2);
        ui->diceLabel_1->setMovie(movie1);
        ui->diceLabel_2->setMovie(movie2);
        movie1->start();
        movie2->start();
        if(d1 != d2) {
            if(e == game::gamePlay::GameInstance::eventType::Dice) {
                QTimer::singleShot(1000, this, [&g, this](){
                    g.notifyUserInput(d1 + d2);
                    ui->diceLabel_1->hide();
                    ui->diceLabel_2->hide();
                    ui->rollDiceButton->hide();
                });
            }
            else {
                g.notifyUserInput(0);
            }
        }
        else {
            d1 = 2 * d2;
            ui->rollDiceButton->setText("Roll dice again");
        }
    }
    else if (ui->rollDiceButton->text() == "Roll dice again"){
        using game::gamePlay::GameInstance;
        auto e = g.getActiveEvent();
        if(e == GameInstance::eventType::Dice) {
            ui->diceLabel_2->hide();
            d3 = rand() % 6 + 1;
            QString gifPath3 = ":/resources/dice" + numberList[d3 - 1] + ".gif";
            QMovie* movie3 = new QMovie(gifPath3);
            ui->diceLabel_1->setMovie(movie3);
            movie3->start();
            if(d2 == d3) {
                QTimer::singleShot(1000, this, [&g, this](){
                    g.notifyUserInput(-3 * d3);
                    ui->diceLabel_1->hide();
                    ui->diceLabel_2->hide();
                    ui->rollDiceButton->hide();
                });
            }
            else {
                QTimer::singleShot(1000, this, [&g, this](){
                    g.notifyUserInput(d1 + d3);
                    ui->diceLabel_1->hide();
                    ui->diceLabel_2->hide();
                    ui->rollDiceButton->hide();
                });
            }
        }
        else {
            d2 = rand() % 6 + 1;
            d3 = rand() % 6 + 1;
            QString gifPath2 = ":/resources/dice" + numberList[d2 - 1] + ".gif";
            QString gifPath3 = ":/resources/dice" + numberList[d3 - 2] + ".gif";
            QMovie* movie2 = new QMovie(gifPath2);
            QMovie* movie3 = new QMovie(gifPath3);
            ui->diceLabel_1->setMovie(movie2);
            ui->diceLabel_2->setMovie(movie3);
            movie2->start();
            movie3->start();
            if(d3 != d2) {
                QTimer::singleShot(1000, this, [&g, this](){
                    g.notifyUserInput(0);
                    ui->diceLabel_1->hide();
                    ui->diceLabel_2->hide();
                    ui->rollDiceButton->hide();
                });
            }
            else {
                d1 += 2 * d2;
                ui->rollDiceButton->setText("Roll dice again and again");
            }
        }
    }
    else {
        d2 = rand() % 6 + 1;
        d3 = rand() % 6 + 1;
        QString gifPath2 = ":/resources/dice" + numberList[d2 - 1] + ".gif";
        QString gifPath3 = ":/resources/dice" + numberList[d3 - 2] + ".gif";
        QMovie* movie2 = new QMovie(gifPath2);
        QMovie* movie3 = new QMovie(gifPath3);
        ui->diceLabel_1->setMovie(movie2);
        ui->diceLabel_2->setMovie(movie3);
        movie2->start();
        movie3->start();
        if(d3 != d2) {
            QTimer::singleShot(1000, this, [&g, this](){
                g.notifyUserInput(0);
                ui->diceLabel_1->hide();
                ui->diceLabel_2->hide();
                ui->rollDiceButton->hide();
            });
        }
        else {
            d1 += 2 * d2;
            QTimer::singleShot(1000, this, [&g, this](){
                g.notifyUserInput(d1);
                ui->diceLabel_1->hide();
                ui->diceLabel_2->hide();
                ui->rollDiceButton->hide();
            });
        }
    }
}

