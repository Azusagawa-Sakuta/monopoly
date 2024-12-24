#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QThread>
#include <QTimer>
#include "../backend/game.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel *mapLabel;
    QLabel *playerInfoLabel;
    QPushButton *refreshButton;

    void printMap();
    void printPlayerPos(game::player::Player* p);

private slots:
    void refresh();
    void callbackPlayerUpdate(game::player::Player* p);
    void callbackBuy(game::player::Player* p, game::gamePlay::Buildable* tile, game::cashType cashToPay);
};

#endif // MAINWINDOW_H
