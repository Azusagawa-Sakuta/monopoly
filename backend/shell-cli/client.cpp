#include "../game.hpp"
#include <iostream>

game::player::Player p1, p2;

void printPlayerInfo(game::player::Player &p) {
    std::cout << "Cash: " << p.getCash() << ", Player position: " << p.getPosition() << std::endl;
}

void refresh() { 
    printPlayerInfo(p1); 
    printPlayerInfo(p2); 
}

int main() {
    game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();
    g.addPlayer(&p1);
    g.addPlayer(&p2);
    g.addTile(new game::gamePlay::Home);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Prison);
    g.addTile(new game::gamePlay::Tax);
    g.callbackPlayerUpdate = [](game::player::Player* p) { refresh(); };
    g.callbackDice = [](game::player::Player* p, int v1, int v2) { std::cout << "Player " << p << " rolled " << v1 << " and " << v2 << std::endl; };
    g.callbackPrison = [](game::player::Player* p, game::gamePlay::Prison* tile) { std::cout << "Player " << p << " is in jail " << game::gamePlay::GameInstance::getInstance().findTile(tile) << std::endl; };
    g.callbackBuy = [](game::player::Player* p, game::gamePlay::Buildable* tile, game::cashType cashToPay) { std::cout << "Player " << p << " bought " << game::gamePlay::GameInstance::getInstance().findTile(tile) << " for " << cashToPay << std::endl; return true; };
    while (1) {
        g.tick();
        system("pause");
    }
    return 0;
}