#include "startwidget.h"

#include <QApplication>
#include "../backend/game.h"
#include "../backend/utils.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Initialize game constants
    game::constant::homeReward = 2500;
    game::constant::defaultTaxRate = 0.1f;
    game::constant::initialCash = 100000;

    // Pre-create the singleton so Logger is ready
    game::gamePlay::GameInstance::getInstance();
    utils::Logger::getInstance().log("main(): Application started.");

    startWidget w;
    w.show();
    return a.exec();
}
