#include "gamemainwidget.h"
#include "ui_gamemainwidget.h"
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QPen>
#include <QBrush>
#include <QResizeEvent>
#include <cmath>
#include <vector>
#include "../backend/game.h"

gameMainWidget::gameMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameMainWidget),
    scene(new QGraphicsScene(this)),
    scenePlayer1(new QGraphicsScene(this)),
    scenePlayer2(new QGraphicsScene(this)),
    scenePlayer3(new QGraphicsScene(this)),
    scenePlayer4(new QGraphicsScene(this))
{
    ui->setupUi(this);
    showMaximized();
    //initializeGameInstance();
    scenePlayer1->clear();
    scenePlayer2->clear();
    scenePlayer3->clear();
    scenePlayer4->clear();
    update();
    int playerNum = loadImage();
    ui->mapView->setScene(scene);
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
    case 3:
        ui->playerAvatarGraphics_4->hide();
        ui->playerInfo_4_1->hide();
        ui->playerInfo_4_2->hide();
        ui->playerInfo_4_3->hide();
        ui->playerNickname_4->hide();
    }
    int computers = 0, players = 0;
    auto& g = game::gamePlay::GameInstance::getInstance();
    const auto& playerList = g.getPlayers();
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

    g.notifyUserInput(0);
}

gameMainWidget::~gameMainWidget()
{
    delete ui;
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
    g.callbackTileUpdate = [this](game::gamePlay::Tile* t) {
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
    update();
}

void gameMainWidget::update() {
    scene->clear(); // Clear old items
    paintMap();
    //paintPlayerInfo();
}

QPixmap gameMainWidget::getTileImage(const game::gamePlay::Tile* tile)
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

QPixmap gameMainWidget::getPlayerIndicator(const game::player::Player* p)
{
    QPixmap img;
    std::string imgs[] = {":/resources/draft/1P.png", ":/resources/draft/2P.png", ":/resources/draft/3P.png", ":/resources/draft/4P.png"};
    img = QPixmap(QString::fromStdString(imgs[p->getPosition()]));
    if (img.isNull()) {
        qDebug() << "Failed to load image";
    }
    return img;
}

void gameMainWidget::paintMap()
{

    int tileW = 50, tileH = 50;
    int spacing = 5;
    int horizontalSpacing = 64 + spacing * 2;
    int verticalSpacing = 32 + spacing;

    // Retrieve tiles from GameInstance
    const std::vector<game::gamePlay::Tile*>& tiles = game::gamePlay::GameInstance::getInstance().getTiles();

    // Define colors for the tiles
    QColor colors[] = { Qt::red, Qt::green, Qt::blue, Qt::yellow };

    // Calculate the number of rows and columns to form a diamond shape
    int numTiles = tiles.size();
    int numCols = static_cast<int>(std::ceil(numTiles / 4.0f)) + 1;
    int numRows = static_cast<int>(std::ceil(numTiles / 2.0f)) - numCols + 2;

    int index = 0;

    QPixmap tileImage(":/resources/draft/tileTemplate.png");
    if (tileImage.isNull()) {
        qDebug() << "Failed to load image";
    }

    auto& players = game::gamePlay::GameInstance::getInstance().getPlayers();

    int offsetX = (- tileImage.width()) / 2;
    int offsetY = (- tileImage.height()) / 2;

    int depth = 0;

    scene->addRect(- tileW / 2.0f, - tileH / 2.0f, tileW, tileH, QPen(Qt::black), QBrush(Qt::blue));

    bool paintTileIndex = true;

    // Left column (bottom to top)
    for (int row = numRows - 1; row >= 0 && index < numTiles; --row) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = horizontalSpacing * (- numRows + row + 1);
        int y = row * verticalSpacing;
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor()];
        }
        //scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(color));
        auto pixmapItem = scene->addPixmap(getTileImage(tile));
        pixmapItem->setPos(x + offsetX, y + offsetY);
        pixmapItem->setZValue(depth--);
        if (paintTileIndex) 
            scene->addText(QString::number(index))->setPos(x, y);
        int i = 0;
        for (auto& a : players) {
            if (a->getPosition() == index - 1) {
                QPixmap playerImage(getPlayerIndicator(a));
                auto item = scene->addPixmap(playerImage);
                item->setPos(x + offsetX, y + offsetY - i * 64);
                item->setZValue(100);
            }
            i++;
        }
    }

    // Top row (left to right)
    for (int col = 1; col < numCols && index < numTiles; ++col) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = horizontalSpacing * (- numRows + col + 1);
        int y = - col * verticalSpacing;
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor()];
        }
        //scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(color));
        auto pixmapItem = scene->addPixmap(getTileImage(tile));
        pixmapItem->setPos(x + offsetX, y + offsetY);
        pixmapItem->setZValue(depth--);
        scene->addText(QString::number(index))->setPos(x, y);
    }

    depth += 2;

    // Right column (top to bottom)
    for (int row = 1; row < numRows && index < numTiles; ++row) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = horizontalSpacing * row;
        int y = (- numRows + row + 1) * verticalSpacing;
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor()];
        }
        //scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(color));
        auto pixmapItem = scene->addPixmap(getTileImage(tile));
        pixmapItem->setPos(x + offsetX, y + offsetY);
        pixmapItem->setZValue(depth++);
        scene->addText(QString::number(index))->setPos(x, y);
    }

    // Bottom row (right to left)
    for (int col = numCols - 2; col > 0 && index < numTiles; --col) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = horizontalSpacing * col;
        int y = (numCols - col - 1) * verticalSpacing;
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor()];
        }
        //scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(color));
        auto pixmapItem = scene->addPixmap(getTileImage(tile));
        pixmapItem->setPos(x + offsetX, y + offsetY);
        pixmapItem->setZValue(depth++);
        scene->addText(QString::number(index))->setPos(x, y);
    }
}

void gameMainWidget::paintPlayerInfo()
{
    int rectWidth = 100, rectHeight = 100;
    QColor playerColors[] = { Qt::red, Qt::green, Qt::blue, Qt::yellow };

    // Top-left corner
    scene->addRect(- this->width() / 2.0f + 2, - this->height() / 2.0f + 2, rectWidth, rectHeight, QPen(Qt::black), QBrush(playerColors[0]));

    // Top-right corner
    scene->addRect(this->width() / 2.0f - rectWidth - 2, - this->height() / 2.0f + 2, rectWidth, rectHeight, QPen(Qt::black), QBrush(playerColors[1]));

    // Bottom-left corner
    scene->addRect(- this->width() / 2.0f + 2, this->height() / 2.0f - rectHeight - 2, rectWidth, rectHeight, QPen(Qt::black), QBrush(playerColors[2]));

    // Bottom-right corner
    scene->addRect(this->width() / 2.0f - rectWidth - 2, this->height() / 2.0f - rectHeight - 2, rectWidth, rectHeight, QPen(Qt::black), QBrush(playerColors[3]));
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
