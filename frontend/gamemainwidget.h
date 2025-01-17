#ifndef GAMEMAINWIDGET_H
#define GAMEMAINWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMovie>
#include <QLabel>
#include <QGraphicsColorizeEffect>
#include <../backend/game.h>

namespace Ui {
class gameMainWidget;
}

struct DicePart {
    QMovie* diceMovie1;
    QMovie* diceMovie2;
    QLabel* diceLabel1;
    QLabel* diceLabel2;
};

class gameMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit gameMainWidget(QWidget *parent = nullptr);
    ~gameMainWidget();

private slots:
    void onTick();

    void on_rollDiceButton_clicked();
    void rollDice(int& d1, int& d2);
    void rollDiceBackdoor(int d1, int d2);

private:
    Ui::gameMainWidget *ui;
    QGraphicsScene *scene; // Scene to show the map
    QGraphicsScene *scenePlayer1;
    QGraphicsScene *scenePlayer2;
    QGraphicsScene *scenePlayer3;
    QGraphicsScene *scenePlayer4;

    QTimer *timer;

    void initializeGameInstance();

    void paintMap(); // Paint the map
    int loadImage();
    void loadDice(int d1, int d2);
    void update();

    void resizeEvent(QResizeEvent* event) override;
    void updatePlayerInfo();
    QPixmap getTileImage(game::gamePlay::Tile* tile);
    QPixmap getHouseImage(game::gamePlay::Tile* tile);
    QPixmap getPlayerIndicator(game::player::Player* p);
    void paintTile(int x, int y, int index, int depth, game::gamePlay::Tile* tile);
    void paintHouse(int x, int y, int depth, QPixmap tileImage);
};

#endif // GAMEMAINWIDGET_H
