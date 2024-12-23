#ifndef GAMEMAINWIDGET_H
#define GAMEMAINWIDGET_H

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class gameMainWidget;
}

class gameMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit gameMainWidget(QWidget *parent = nullptr, int len = 0);
    ~gameMainWidget();

private slots:

private:
    Ui::gameMainWidget *ui;
    QGraphicsScene *scene; // Scene to show the map
    int mapLength;

    void generateMap(int n); // Helper to create 4n tiles (up, down, left, right)
};

#endif // GAMEMAINWIDGET_H
