#include "gamemainwidget.h"
#include "ui_gamemainwidget.h"
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QPen>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QColor>
#include <QPointF>
#include <QPolygonF>
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
    int tileW = 50, tileH = 50, tileD = 25; // Adding depth for the cube
    int spacing = 5;

    // Retrieve tiles from GameInstance
    const std::vector<game::gamePlay::Tile*>& tiles = game::gamePlay::GameInstance::getInstance().getTiles();

    // Define colors for the tiles
    QColor colors[] = { Qt::red, Qt::green, Qt::blue, Qt::yellow };

    // Calculate the number of rows and columns to form a hollow rectangle
    int numTiles = tiles.size();
    int numCols = static_cast<int>(std::ceil(numTiles / 4.0f)) + 1;
    int numRows = static_cast<int>(std::ceil(numTiles / 2.0f)) - numCols + 2;

    int index = 0;

    auto addCube = [&](int x, int y, QColor color, int tileNumber) {
        // Calculate points for the cube
        QPointF topFrontLeft(x, y);
        QPointF topFrontRight(x + tileW, y);
        QPointF bottomFrontLeft(x, y + tileH);
        QPointF bottomFrontRight(x + tileW, y + tileH);

        QPointF topBackLeft(x + tileD, y - tileD);
        QPointF topBackRight(x + tileW + tileD, y - tileD);
        QPointF bottomBackLeft(x + tileD, y + tileH - tileD);
        QPointF bottomBackRight(x + tileW + tileD, y + tileH - tileD);

        // Draw front face
        scene->addPolygon(QPolygonF({ topFrontLeft, topFrontRight, bottomFrontRight, bottomFrontLeft }), QPen(Qt::black), QBrush(color));

        // Draw top face
        scene->addPolygon(QPolygonF({ topFrontLeft, topFrontRight, topBackRight, topBackLeft }), QPen(Qt::black), QBrush(color.lighter()));

        // Draw side face
        scene->addPolygon(QPolygonF({ topFrontRight, bottomFrontRight, bottomBackRight, topBackRight }), QPen(Qt::black), QBrush(color.darker()));

        // Add text label on the top face
        QGraphicsTextItem* textItem = scene->addText(QString::number(tileNumber));
        textItem->setDefaultTextColor(Qt::black);
        textItem->setPos((topFrontLeft + topBackRight) / 2 - QPointF(tileW / 4, tileH / 4));
    };

    // Top row
    for (int col = 0; col < numCols && index < numTiles; ++col) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = col * (tileW + spacing);
        int y = 0;
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor() % 4];
        }
        addCube(x, y, color, index);
        index++;
    }

    // Right column
    for (int row = 1; row < numRows - 1 && index < numTiles; ++row) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = (numCols - 1) * (tileW + spacing);
        int y = row * (tileH + spacing);
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor() % 4];
        }
        addCube(x, y, color, index);
        index++;
    }

    // Bottom row
    for (int col = numCols - 1; col >= 0 && index < numTiles; --col) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = col * (tileW + spacing);
        int y = (numRows - 1) * (tileH + spacing);
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor() % 4];
        }
        addCube(x, y, color, index);
        index++;
    }

    // Left column
    for (int row = numRows - 2; row > 0 && index < numTiles; --row) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = 0;
        int y = row * (tileH + spacing);
        QColor color = Qt::gray;
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
            game::gamePlay::Buildable* buildableTile = static_cast<game::gamePlay::Buildable*>(tile);
            color = colors[buildableTile->getColor() % 4];
        }
        addCube(x, y, color, index);
        index++;
    }
}
