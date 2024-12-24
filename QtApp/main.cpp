#include "mainwindow.hpp"
#include <QApplication>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <thread>
#include "../backend/game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      mapLabel(new QLabel(this)),
      playerInfoLabel(new QLabel(this)),
      refreshButton(new QPushButton("Refresh", this))
{
    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mapLabel);
    layout->addWidget(playerInfoLabel);
    layout->addWidget(refreshButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Connect refresh button click to refresh function
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refresh);

    // Setting up a timer to refresh the UI periodically
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::refresh);
    timer->start(1000);  // Refresh every second

    // Game setup
    game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();
    g.callbackPlayerUpdate = std::bind(&MainWindow::callbackPlayerUpdate, this, std::placeholders::_1);
    g.callbackBuy = std::bind(&MainWindow::callbackBuy, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

MainWindow::~MainWindow() {}

void MainWindow::printMap() {
    // This method should convert the map state into text, and update the mapLabel
    std::string mapText;
    int colorList[] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_GREEN, FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_RED, FOREGROUND_RED | FOREGROUND_BLUE, FOREGROUND_GREEN | FOREGROUND_BLUE };
    char shortList[] = {'X', 'H', 'B', 'P', 'T', 'R'};

    for (auto &tile : game::gamePlay::GameInstance::getInstance().getTiles()) {
        // Generate text for each tile
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable && static_cast<game::gamePlay::Buildable*>(tile)->isOwned()) {
            mapText += std::to_string(game::gamePlay::GameInstance::getInstance().findPlayerPos(static_cast<game::gamePlay::Buildable*>(tile)->getOwner())) + " ";
        } else {
            mapText += std::string(1, shortList[tile->getType()]) + " ";
        }
    }

    mapLabel->setText(QString::fromStdString(mapText));
}

void MainWindow::printPlayerPos(game::player::Player* p) {
    // Display player info
    std::string playerInfo = "Player " + std::to_string(game::gamePlay::GameInstance::getInstance().findPlayerPos(p)) +
                             " Cash: " + std::to_string(p->getCash()) + "$, Pos: " + std::to_string(p->getPosition());
    playerInfoLabel->setText(QString::fromStdString(playerInfo));
}

void MainWindow::refresh() {
    // Refresh game UI
    printMap();
    for (auto &p : game::gamePlay::GameInstance::getInstance().getPlayers()) {
        printPlayerPos(p);
    }
}

void MainWindow::callbackPlayerUpdate(game::player::Player* p) {
    // Update message when player updates
    QMessageBox::information(this, "Player Update", "Player " + QString::fromStdString(game::gamePlay::GameInstance::getInstance().findPlayerPos(p)) + " updated.");
}

void MainWindow::callbackBuy(game::player::Player* p, game::gamePlay::Buildable* tile, game::cashType cashToPay) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Purchase", 
                                  "Should player " + QString::fromStdString(game::gamePlay::GameInstance::getInstance().findPlayerPos(p)) +
                                  " buy " + QString::fromStdString(game::gamePlay::GameInstance::getInstance().findTile(tile)) +
                                  " for " + QString::number(cashToPay) + "?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        game::gamePlay::GameInstance::getInstance().notifyUserInput(true);
    } else {
        game::gamePlay::GameInstance::getInstance().notifyUserInput(false);
    }
}

int main(int argc, char *argv[])
{
    std::thread gameThread([]() {
        int playerCount = 4;
        game::constant::homeReward = 2500;
        game::constant::defaultTaxRate = 0.1f;
        game::constant::initialCash = 10000;
        game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();

        for (int i = 0; i < playerCount; i++) {
            game::gamePlay::GameInstance::getInstance().addPlayer(new game::player::Player);
        }

        g.addTile(new game::gamePlay::Home);
        g.addTile(new game::gamePlay::Buildable);
    });

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    gameThread.detach();
    return a.exec();
}
