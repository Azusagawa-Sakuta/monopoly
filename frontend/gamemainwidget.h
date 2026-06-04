#ifndef GAMEMAINWIDGET_H
#define GAMEMAINWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMovie>
#include <QLabel>
#include <QGraphicsColorizeEffect>
#include <../backend/game.h>

namespace Ui {
class gameMainWidget;
}

struct DicePart {
    QMovie* diceMovie1;
    QMovie* diceMovie2;
    QLabel* diceLabel1;
    QLabel* diceLabel2;
};

class gameMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit gameMainWidget(QWidget *parent = nullptr);
    ~gameMainWidget();

private slots:
    // Dice button (still user-driven)
    void on_rollDiceButton_clicked();

    // Signal handlers from GameInstance
    void onTurnStarted(game::player::Player* player);
    void onGameEnded(game::player::Player* winner);
    void onDiceRolled(game::player::Player* player, int dice1, int dice2);
    void onPlayerMoved(game::player::Player* player, int fromPos, int toPos);
    void onPlayerPassedGo(game::player::Player* player, game::cashType reward);
    void onBuyDecisionNeeded(game::player::Player* player, game::gamePlay::Buildable* tile, game::cashType cost);
    void onAuctionNeeded(game::gamePlay::Buildable* tile, game::cashType reservePrice, game::cashType bidIncrement);
    void onBuildDecisionNeeded(game::player::Player* player, game::gamePlay::Buildable* tile, game::cashType houseCost, int maxLevels);
    void onSellDecisionNeeded(game::player::Player* player, game::cashType amountNeeded);
    void onRentPaid(game::player::Player* payer, game::player::Player* owner, game::gamePlay::Buildable* tile, game::cashType amount);
    void onTaxPaid(game::player::Player* player, game::gamePlay::Tax* tile, game::cashType amount);
    void onHomeRewardCollected(game::player::Player* player, game::cashType amount);
    void onPlayerPrisoned(game::player::Player* player, game::gamePlay::Prison* tile);
    void onPrisonDiceNeeded(game::player::Player* player);
    void onPrisonPayOutNeeded(game::player::Player* player, game::cashType cost);
    void onRandomEventTriggered(game::gamePlay::Tile* tile, const QString& description);
    void onPlayerUpdated(game::player::Player* player);
    void onPlayerBankrupted(game::player::Player* player);
    void onPropertyChanged(game::gamePlay::Buildable* tile);
    void onBoardUpdateNeeded();

private:
    Ui::gameMainWidget *ui;
    QGraphicsScene *scene;
    QGraphicsScene *scenePlayer1;
    QGraphicsScene *scenePlayer2;
    QGraphicsScene *scenePlayer3;
    QGraphicsScene *scenePlayer4;

    int firstDiceNumber;
    bool isPrisonDiceMode;

    void connectGameSignals();
    void paintMap();
    int loadImage();
    void rollDice(int& d1, int& d2);
    void rollDiceBackdoor(int d1, int d2);
    void loadDice(int d1, int d2);
    void update();
    void updatePlayerInfo();

    void resizeEvent(QResizeEvent* event) override;
    QPixmap getTileImage(game::gamePlay::Tile* tile);
    QPixmap getHouseImage(game::gamePlay::Tile* tile);
    QPixmap getPlayerIndicator(game::player::Player* p);
    void paintTile(int x, int y, int index, int depth, game::gamePlay::Tile* tile);
    void paintHouse(int x, int y, int depth, QPixmap tileImage);
};

#endif // GAMEMAINWIDGET_H
