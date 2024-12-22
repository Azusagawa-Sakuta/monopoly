#include <QApplication>
#include <QLabel>
#include <thread>
#include "../backend/game.hpp"

void event() {

}

void gameInitialization() {
    game::constant::homeReward = 2500;
    game::constant::defaultTaxRate = 0.1f;
    game::constant::initialCash = 10000;
    game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();
    
    for (int i = 0; i < 4; i++) 
        g.addPlayer(new game::player::Player);
    
    g.addTile(new game::gamePlay::Home);
    g.addTile(new game::gamePlay::Buildable);
}

int main(int argc, char *argv[]) {

    gameInitialization();
    std::thread t([]() {
        game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();
        while (1)
            g.tick();
    });

    QApplication app(argc, argv);
    QLabel label("Hello, World!");
    label.setAlignment(Qt::AlignCenter);
    label.resize(200, 100);
    label.show();

    // After UI initialization
    t.detach();

    return app.exec();
}
