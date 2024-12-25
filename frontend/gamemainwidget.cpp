#include "gamemainwidget.h"
#include "ui_gamemainwidget.h"
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QPen>
#include <QBrush>
#include <cmath>
#include "../backend/game.h"

gameMainWidget::gameMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameMainWidget),
    scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    generateMap();
    ui->graphicsView->setScene(scene);
}

gameMainWidget::~gameMainWidget()
{
    delete ui;
}

void gameMainWidget::generateMap()
{
    scene->clear(); // Clear old items

    //int tileW = 50, tileH = 50;
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
        auto pixmapItem = scene->addPixmap(tileImage);
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
        auto pixmapItem = scene->addPixmap(tileImage);
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
        auto pixmapItem = scene->addPixmap(tileImage);
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
        auto pixmapItem = scene->addPixmap(tileImage);
        pixmapItem->setPos(x + offsetX, y + offsetY);
        pixmapItem->setZValue(depth++);
        scene->addText(QString::number(index))->setPos(x, y);
    }
}