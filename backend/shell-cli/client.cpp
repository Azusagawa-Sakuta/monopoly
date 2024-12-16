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

void callbackPlayerUpdate(game::player::Player* p) { 
    refresh(); 
}

bool callbackBuy(game::player::Player* p, game::gamePlay::Buildable* tile, game::cashType cashToPay) {
    std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " bought " << game::gamePlay::GameInstance::getInstance().findTile(tile) << " for " << cashToPay << std::endl;
    return true; 
};

int main() {
    game::constant::homeReward = 1000;
    game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();
    // Initializing
    g.addPlayer(&p1);
    g.addPlayer(&p2);
    
    g.addTile(new game::gamePlay::Home);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Prison);
    g.addTile(new game::gamePlay::Tax);

    // Setting callback functions.
    // Using normal functions:
    g.callbackPlayerUpdate = callbackPlayerUpdate;
    // Using lambda functions:
    g.callbackDice = [](game::player::Player* p, int v1, int v2) { std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " rolled " << v1 << " and " << v2 << std::endl; };
    g.callbackDice3rd = [](game::player::Player* p, int v3) { std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " rolled an additional " << v3 << std::endl; };
    g.callbackPrison = [](game::player::Player* p, game::gamePlay::Prison* tile) { std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " is in jail " << game::gamePlay::GameInstance::getInstance().findTile(tile) << std::endl; };
    g.callbackBuy = callbackBuy;
    g.callbackHomeReward = [](game::player::Player* p, game::gamePlay::Home* tile, game::cashType cash) { std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " received " << cash << " for passing home " << game::gamePlay::GameInstance::getInstance().findTile(tile) << std::endl; };
    g.callbackRent = [](game::player::Player* p, game::gamePlay::Buildable* tile, game::cashType cash) { std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " paid " << cash << " in tile " << game::gamePlay::GameInstance::getInstance().findTile(tile) << " to " << game::gamePlay::GameInstance::getInstance().findPlayerPos(tile->getOwner()) << std::endl; };
    // Main loop
    while (1) {
        g.tick();
        std::cin.get();
    }
    return 0;
}