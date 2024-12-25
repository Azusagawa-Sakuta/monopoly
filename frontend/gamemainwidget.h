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

private:
    Ui::gameMainWidget *ui;
    QGraphicsScene *scene; // Scene to show the map

    void paintMap(); // Paint the map
    void paintPlayerInfo(); // Paint player info

    void resizeEvent(QResizeEvent* event) override;
    QPixmap getTileImage(const game::gamePlay::Tile* tile);
};

#endif // GAMEMAINWIDGET_H
