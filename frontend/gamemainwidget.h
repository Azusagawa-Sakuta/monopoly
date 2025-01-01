#ifndef GAMEMAINWIDGET_H
#define GAMEMAINWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <../backend/game.h>

namespace Ui {
class gameMainWidget;
}

class gameMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit gameMainWidget(QWidget *parent = nullptr);
    ~gameMainWidget();

private slots:
    void onTick();

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
    void update();

    void resizeEvent(QResizeEvent* event) override;
    void updatePlayerInfo();
    QPixmap getTileImage(game::gamePlay::Tile* tile);
    QPixmap getPlayerIndicator(game::player::Player* p);
    void paintTile(int x, int y, int index, int depth, QPixmap tileImage);
};

#endif // GAMEMAINWIDGET_H
