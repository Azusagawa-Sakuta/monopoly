#include "startwidget.h"

#include <QApplication>
#include <QtAssert>
#include <QPropertyBindingSourceLocation>

#include "../backend/game.h"
#include "../backend/utils.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Initialize game constants
    game::constant::homeReward = 2500;
    game::constant::defaultTaxRate = 0.1f;
    game::constant::initialCash = 100000;

    Q_ASSERT(game::constant::initialCash >= 5 * game::constant::defaultBasicRent);
    Q_ASSERT(game::constant::defaultTaxRate < 1.0f && game::constant::defaultTaxRate > 0.0f);

    // Pre-create the singleton so Logger is ready
    game::gamePlay::GameInstance::getInstance();
    utils::Logger::getInstance().log("main(): Application started.");

    startWidget w;
    w.show();
    return a.exec();
}
