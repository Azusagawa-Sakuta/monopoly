#include "startWidget.h"

#include <QApplication>
#include <thread>

int main(int argc, char *argv[]) {
    std::thread([]() {
        int playerCount = 4;
        game::constant::homeReward = 2500;
        game::constant::defaultTaxRate = 0.1f;
        game::constant::initialCash = 10000;
        game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();
    }).detach();
    
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
