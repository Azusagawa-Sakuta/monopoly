#include "startwidget.h"

#include <QApplication>
#include <QThread>
#include "../backend/game.h"


class GameThread : public QThread {
protected:
    void run() override {
        game::constant::homeReward = 2500;
        game::constant::defaultTaxRate = 0.1f;
        game::constant::initialCash = 10000;
        game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();

        g.waitForUserInput();
        while (true) {
            //g.tick();
            QThread::sleep(1);
        }
    }
};


int main(int argc, char *argv[]) {

    GameThread gameThread;
    gameThread.start();
    /*
    game::constant::homeReward = 2500;
    game::constant::defaultTaxRate = 0.1f;
    game::constant::initialCash = 10000;
    game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();

    g.waitForUserInput();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&g]() {
        g.tick();
    });
    timer.start(1000); // Call g.tick() every 1000 milliseconds (1 second)
    */
    QApplication a(argc, argv);
    startWidget w;
    w.show();
    return a.exec();
}
