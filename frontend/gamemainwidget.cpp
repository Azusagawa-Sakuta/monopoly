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

    // Basic tile size and spacing
    int tileW = 50, tileH = 50;
    int spacing = 5;

    // Retrieve tiles from GameInstance
    const std::vector<game::gamePlay::Tile*>& tiles = game::gamePlay::GameInstance::getInstance().getTiles();

    // Define colors for the tiles
    QColor colors[] = { Qt::red, Qt::green, Qt::blue, Qt::yellow };

    // Calculate the number of rows and columns to form a hollow rectangle
    int numTiles = tiles.size();
    int numCols = static_cast<int>(std::ceil(std::sqrt(numTiles)));
    int numRows = (numTiles + numCols - 1) / numCols; // Ceiling division

    int index = 0;

    // Top row
    for (int col = 0; col < numCols && index < numTiles; ++col) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = col * (tileW + spacing);
        int y = 0;
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor() % 4];
        }
        scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(color));
    }

    // Right column
    for (int row = 1; row < numRows - 1 && index < numTiles; ++row) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = (numCols - 1) * (tileW + spacing);
        int y = row * (tileH + spacing);
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor() % 4];
        }
        scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(color));
    }

    // Bottom row
    for (int col = numCols - 1; col >= 0 && index < numTiles; --col) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = col * (tileW + spacing);
        int y = (numRows - 1) * (tileH + spacing);
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor() % 4];
        }
        scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(color));
    }

    // Left column
    for (int row = numRows - 2; row > 0 && index < numTiles; --row) {
        game::gamePlay::Tile* tile = tiles[index++];
        int x = 0;
        int y = row * (tileH + spacing);
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor() % 4];
        }
        scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(color));
    }
}
