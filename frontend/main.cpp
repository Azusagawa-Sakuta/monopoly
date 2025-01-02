#include "startwidget.h"

#include <QApplication>
#include <QThread>
#include "../backend/game.h"
#include "../backend/utils.h"


class GameThread : public QThread {
protected:
    void run() override {
        game::constant::homeReward = 2500;
        game::constant::defaultTaxRate = 0.1f;
        game::constant::initialCash = 114514;
        game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();

        utils::Logger::getInstance().log("main(): Waiting for the game to start...");
        g.waitForUserInput(game::gamePlay::GameInstance::eventType::None);
        utils::Logger::getInstance().log("main(): Game started.");
        while (true) {
            utils::Logger::getInstance().log("main(): Ticking...");
            g.tick();
            QThread::sleep(1);
        }
    }
};


int main(int argc, char *argv[]) {
    GameThread th;
    th.start();
    QApplication a(argc, argv);
    startWidget w;
    w.show();
    return a.exec();
}
