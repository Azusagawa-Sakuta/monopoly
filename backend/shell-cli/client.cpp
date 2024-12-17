#include "../game.hpp"
#include <iostream>
#include <iomanip>
#include <windows.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void printMap() {
    int colorList[] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_GREEN, FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_RED, FOREGROUND_RED | FOREGROUND_BLUE, FOREGROUND_GREEN | FOREGROUND_BLUE };
    char shortList[] = {'X', 'H', 'B', 'P', 'T', 'R'};
    for (auto &tile : game::gamePlay::GameInstance::getInstance().getTiles()) {
        if (tile->getType() == game::gamePlay::Tile::TileType::buildable && static_cast<game::gamePlay::Buildable*>(tile)->isOwned()) 
            SetConsoleTextAttribute(hConsole, colorList[tile->getType()] | FOREGROUND_INTENSITY);
        else
            SetConsoleTextAttribute(hConsole, colorList[tile->getType()] & ~FOREGROUND_INTENSITY);
        std::cout << (tile->getType() == game::gamePlay::Tile::TileType::buildable && static_cast<game::gamePlay::Buildable*>(tile)->isOwned() 
            ? std::to_string(game::gamePlay::GameInstance::getInstance().findPlayerPos(static_cast<game::gamePlay::Buildable*>(tile)->getOwner())) : std::string() + shortList[tile->getType()]) << " ";
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << std::endl;
}

void printPlayerPos(game::player::Player* p) {
    for (int i = 0; i < game::gamePlay::GameInstance::getInstance().getTiles().size(); i++) {
        if (i == p->getPosition()) 
            std::cout << "^ ";
        else
            std::cout << "  ";
    }
    std::cout << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " " << std::setw(5) << std::right << p->getCash() << "$, Pos: " << p->getPosition() << std::endl;
}

void refresh() { 
    system("cls");
    printMap();
    for (auto &p : game::gamePlay::GameInstance::getInstance().getPlayers()) 
        printPlayerPos(p);
}

void callbackPlayerUpdate(game::player::Player* p) { 
    std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " updated" << std::endl;
}

bool callbackBuy(game::player::Player* p, game::gamePlay::Buildable* tile, game::cashType cashToPay) {
    std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " bought " << game::gamePlay::GameInstance::getInstance().findTile(tile) << " for " << cashToPay << std::endl;
    return true; 
};

int main() {
    int playerCount = 4;
    game::constant::homeReward = 2500;
    game::constant::defaultTaxRate = 0.1f;
    game::constant::initialCash = 10000;
    game::gamePlay::GameInstance &g = game::gamePlay::GameInstance::getInstance();
    
    for (int i = 0; i < playerCount; i++) 
        game::gamePlay::GameInstance::getInstance().addPlayer(new game::player::Player);
    
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
    g.callbackRent = [](game::player::Player* p, game::gamePlay::Buildable* tile, game::cashType cash) { std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " paid " << cash << " to " << game::gamePlay::GameInstance::getInstance().findPlayerPos(tile->getOwner()) << " in tile " << game::gamePlay::GameInstance::getInstance().findTile(tile) << std::endl; };
    g.callbackTax = [](game::player::Player* p, game::gamePlay::Tax* tile, game::cashType cash) { std::cout << "Player " << game::gamePlay::GameInstance::getInstance().findPlayerPos(p) << " paid " << cash << " in tax " << game::gamePlay::GameInstance::getInstance().findTile(tile) << std::endl; };
    g.callbackAuction = [](game::gamePlay::Buildable* tile, game::cashType reservePrice, game::cashType bidIncrement) { 
        std::cout << "Auctioning tile " << game::gamePlay::GameInstance::getInstance().findTile(tile) << " with reserve price " << reservePrice << " and bid increment " << bidIncrement << std::endl;
        std::cout << "However, no one participated :D" << std::endl;
        return std::make_pair(0, nullptr);
    };
    // Main loop
    while (1) {
        refresh();
        g.tick();
        system("pause");
    }
    return 0;
}