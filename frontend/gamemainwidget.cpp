#include "gamemainwidget.h"
#include "ui_gamemainwidget.h"

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QPen>
#include <QBrush>

gameMainWidget::gameMainWidget(QWidget *parent, int len) :
    QWidget(parent),
    ui(new Ui::gameMainWidget),
    scene(new QGraphicsScene(this)),
    mapLength(len)
{
    ui->setupUi(this);
    generateMap(len);
    ui->graphicsView->setScene(scene);
}

gameMainWidget::~gameMainWidget()
{
    delete ui;
}

void gameMainWidget::generateMap(int n)
{
    scene->clear(); // Clear old items

    // Basic tile size and spacing
    int tileW = 50, tileH = 50;
    int spacing = 5;

    // Top row (0..n-1)
    for (int i = 0; i < n; i++) {
        int x = i * (tileW + spacing);
        int y = 0;
        scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(Qt::lightGray));
    }

    // Right column (n..2n-1)
    for (int i = 0; i < n; i++) {
        int x = (n - 1) * (tileW + spacing);
        int y = i * (tileH + spacing);
        scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(Qt::lightGray));
    }

    // Bottom row (2n..3n-1)
    for (int i = 0; i < n; i++) {
        int x = (n - 1 - i) * (tileW + spacing);
        int y = (n - 1) * (tileH + spacing);
        scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(Qt::lightGray));
    }

    // Left column (3n..4n-1)
    for (int i = 0; i < n; i++) {
        int x = 0;
        int y = (n - 1 - i) * (tileH + spacing);
        scene->addRect(x, y, tileW, tileH, QPen(Qt::black), QBrush(Qt::lightGray));
    }
}
