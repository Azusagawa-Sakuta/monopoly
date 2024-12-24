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
#include <QTransform>
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
        // Offset to create isometric view
        int isoX = x - y;
        int isoY = (x + y) / 2;

        // Calculate points for the cube
        QPointF topFrontLeft(isoX, isoY);
        QPointF topFrontRight(isoX + tileW, isoY);
        QPointF bottomFrontLeft(isoX, isoY + tileH);
        QPointF bottomFrontRight(isoX + tileW, isoY + tileH);

        QPointF topBackLeft(isoX + tileD, isoY - tileD);
        QPointF topBackRight(isoX + tileW + tileD, isoY - tileD);
        QPointF bottomBackLeft(isoX + tileD, isoY + tileH - tileD);
        QPointF bottomBackRight(isoX + tileW + tileD, isoY + tileH - tileD);

        // Draw front face
        scene->addPolygon(QPolygonF({ topFrontLeft, topFrontRight, bottomFrontRight, bottomFrontLeft }), QPen(Qt::black), QBrush(color));

        // Draw top face
        scene->addPolygon(QPolygonF({ topFrontLeft, topFrontRight, topBackRight, topBackLeft }), QPen(Qt::black), QBrush(color.lighter()));

        // Draw side face
        scene->addPolygon(QPolygonF({ topFrontRight, bottomFrontRight, bottomBackRight, topBackRight }), QPen(Qt::black), QBrush(color.darker()));

        // Add text label on the top face
        QGraphicsTextItem* textItem = scene->addText(QString::number(tileNumber));
        textItem->setDefaultTextColor(Qt::black);
        QPointF labelPos = (topFrontLeft + topBackRight) / 2 - QPointF(tileW / 4, tileH / 4);
        textItem->setPos(labelPos);
    };

    // Top row
    for (int col = 0; col < numCols && index < numTiles; ++col) {
        game::gamePlay::Tile* tile = tiles[index];
        int x = col * (tileW + spacing + tileD);
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
        int x = (numCols - 1) * (tileW + spacing + tileD);
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
        int x = col * (tileW + spacing + tileD);
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
