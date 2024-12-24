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
    explicit gameMainWidget(QWidget *parent = nullptr);
    ~gameMainWidget();

private slots:

private:
    Ui::gameMainWidget *ui;
    QGraphicsScene *scene; // Scene to show the map

    void generateMap(); // Helper to create 4n tiles (up, down, left, right)
};

#endif // GAMEMAINWIDGET_H
