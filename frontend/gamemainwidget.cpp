#include "gamemainwidget.h"
#include "ui_gamemainwidget.h"
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QPen>
#include <QBrush>
#include <QResizeEvent>
#include <cmath>
#include "../backend/game.h"

gameMainWidget::gameMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameMainWidget),
    scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    showMaximized();
    scene->clear(); // Clear old items
    paintMap();
    //paintPlayerInfo();
    ui->mapView->setScene(scene);
}

gameMainWidget::~gameMainWidget()
{
    delete ui;
}

void gameMainWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    QSize newSize = event->size();
    // 更新 QGraphicsView 的大小以填充窗口
    ui->mapView->setGeometry(0, 0, newSize.width(), newSize.height());
    scene->clear(); // Clear old items
    paintMap();
    paintPlayerInfo();
}

QPixmap gameMainWidget::getTileImage(const game::gamePlay::Tile* tile)
{
    QPixmap tileImage;
    if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
        const game::gamePlay::Buildable* buildableTile = static_cast<const game::gamePlay::Buildable*>(tile);
        std::string colors[] = {"../../resources/draft/tileRed.png", "../../resources/draft/tileGreen.png", "../../resources/draft/tileBlue.png", "../../resources/draft/tileYellow.png"};
        tileImage = QPixmap(QString::fromStdString(colors[buildableTile->getColor()]));
    } else {
        tileImage = QPixmap("../../resources/draft/tileTemplate.png");
    }
    if (tileImage.isNull()) {
        qDebug() << "Failed to load image";
    }
    return tileImage;
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

    QPixmap tileImage("../../resources/draft/tileTemplate.png");
    if (tileImage.isNull()) {
        qDebug() << "Failed to load image";
    }

    int offsetX = (- tileImage.width()) / 2;
    int offsetY = (- tileImage.height()) / 2;

    int depth = 0;

    scene->addRect(- tileW / 2.0f, - tileH / 2.0f, tileW, tileH, QPen(Qt::black), QBrush(Qt::blue));

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
        scene->addText(QString::number(index))->setPos(x, y);
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