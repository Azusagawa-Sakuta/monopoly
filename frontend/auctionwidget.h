#ifndef AUCTIONWIDGET_H
#define AUCTIONWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include "../backend/game.h"

namespace Ui {
class auctionWidget;
}

class auctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit auctionWidget(QWidget *parent = nullptr);
    ~auctionWidget();

private:
    Ui::auctionWidget *ui;
    QGraphicsScene *sceneTile;
    QGraphicsScene *scenePlayer1;
    QGraphicsScene *scenePlayer2;
    QGraphicsScene *scenePlayer3;
    QGraphicsScene *scenePlayer4;

    int currentBid;
    int currentPlayerIndex;
    int maxBidPlayerIndex;
    int round;

    game::gamePlay::GameInstance::auctionRequest req;

    QPixmap getTileImage(game::gamePlay::Tile* tile);
    void nextPlayer();

private slots:
    void on_bidButton_clicked();
    void on_passButton_clicked();

};

#endif // AUCTIONWIDGET_H
