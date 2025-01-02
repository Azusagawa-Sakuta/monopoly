#include "auctionwidget.h"
#include "ui_auctionwidget.h"
#include "../backend/game.h"
#include <QGraphicsColorizeEffect>

auctionWidget::auctionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::auctionWidget),
    sceneTile(new QGraphicsScene(this)),
    scenePlayer1(new QGraphicsScene(this)),
    scenePlayer2(new QGraphicsScene(this)),
    scenePlayer3(new QGraphicsScene(this)),
    scenePlayer4(new QGraphicsScene(this))
{
    ui->setupUi(this);
}

void auctionWidget::initialize() {
    sceneTile->clear();
    scenePlayer1->clear();
    scenePlayer2->clear();
    scenePlayer3->clear();
    scenePlayer4->clear();
    ui->tileGraphics->setScene(sceneTile);

    ui->playerAvatar_1->setScene(scenePlayer1);
    ui->playerAvatar_2->setScene(scenePlayer2);
    ui->playerAvatar_3->setScene(scenePlayer3);
    ui->playerAvatar_4->setScene(scenePlayer4);
    
    auto& g = game::gamePlay::GameInstance::getInstance();
    const auto& playerList = g.getPlayers();
    
    int i = 0;
    for (const auto& it : playerList) {
        i++;
        QPixmap pixmap(QString::fromStdString(it->getImagePath()));
        QPixmap scaledPixmap = pixmap.scaled(512, 512, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(scaledPixmap);
        switch(i) {
        case 1:
            scenePlayer1->addItem(item);
            item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
            ui->playerAvatar_1->fitInView(scenePlayer1->sceneRect(), Qt::KeepAspectRatio);
            ui->playerLabel_1->setText(QString::fromStdString(it->getNickname()));
            ui->playerValue_1->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            break;
        case 2:
            scenePlayer2->addItem(item);
            item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
            ui->playerAvatar_2->fitInView(scenePlayer2->sceneRect(), Qt::KeepAspectRatio);
            ui->playerLabel_2->setText(QString::fromStdString(it->getNickname()));
            ui->playerValue_2->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            break;
        case 3:
            scenePlayer3->addItem(item);
            item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
            ui->playerAvatar_3->fitInView(scenePlayer3->sceneRect(), Qt::KeepAspectRatio);
            ui->playerLabel_3->setText(QString::fromStdString(it->getNickname()));
            ui->playerValue_3->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
            break;
        case 4:
            scenePlayer4->addItem(item);
            item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
            ui->playerAvatar_4->fitInView(scenePlayer4->sceneRect(), Qt::KeepAspectRatio);
            ui->playerLabel_4->setText(QString::fromStdString(it->getNickname()));
            ui->playerValue_4->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
        }
    }

    req = std::any_cast<game::gamePlay::GameInstance::auctionRequest>(g.getActiveEventParam());

    QPixmap pixmap(getTileImage(req.tile));
    QPixmap scaledPixmap = pixmap.scaled(256, 256, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(scaledPixmap);

    sceneTile->addItem(item);
    item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
    ui->tileGraphics->fitInView(sceneTile->sceneRect(), Qt::KeepAspectRatioByExpanding);
    
    ui->tileLabel->setText(QString::fromStdString("Tile " + std::to_string(g.findTile(req.tile)) + " is being auctioned, with minimum $" + std::to_string(req.reservePrice) + " and increment $" + std::to_string(req.bidIncrement) + "."));

    maxBidPlayerIndex = -1;
    currentPlayerIndex = -1;
    currentBid = 0;
    round = 0;

    ui->bidInput->setMinimum(req.reservePrice);
    ui->bidInput->setSingleStep(req.bidIncrement);
    ui->bidInput->setMaximum(114514);
    nextPlayer();
}

auctionWidget::~auctionWidget()
{
    delete ui;
}

QPixmap auctionWidget::getTileImage(game::gamePlay::Tile* tile)
{
    QPixmap tileImage;
    if (tile->getType() == game::gamePlay::Tile::TileType::buildable) {
        const game::gamePlay::Buildable* buildableTile = static_cast<const game::gamePlay::Buildable*>(tile);
        std::string colors[] = {":/resources/draft/tileRed.png", ":/resources/draft/tileGreen.png", ":/resources/draft/tileBlue.png", ":/resources/draft/tileYellow.png"};
        tileImage = QPixmap(QString::fromStdString(colors[buildableTile->getColor()]));
    } else {
        tileImage = QPixmap(":/resources/draft/tileTemplate.png");
    }
    if (tileImage.isNull()) {
        qDebug() << "Failed to load image";
    }
    return tileImage;
}

void auctionWidget::nextPlayer() {
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % game::gamePlay::GameInstance::getInstance().getPlayers().size();
        round++;
    }
    while (game::gamePlay::GameInstance::getInstance().getPlayers()[currentPlayerIndex]->isBankrupted());
    if (round == game::gamePlay::GameInstance::getInstance().getPlayers().size() + 1 && maxBidPlayerIndex == -1) {
        QMessageBox::information(this, "Auction Ended", QString::fromStdString("No one bid for tile " + std::to_string(game::gamePlay::GameInstance::getInstance().findTile(req.tile)) + "."));
        game::gamePlay::GameInstance::getInstance().notifyUserInput(game::gamePlay::GameInstance::auctionResult{0, nullptr});
        close();
    }

    if (maxBidPlayerIndex == currentPlayerIndex) {
        QMessageBox::information(this, "Auction Ended", QString::fromStdString("Player " + std::to_string(maxBidPlayerIndex + 1) + " won the auction for $" + std::to_string(currentBid) + "."));
        game::gamePlay::GameInstance::getInstance().notifyUserInput(game::gamePlay::GameInstance::auctionResult{currentBid, game::gamePlay::GameInstance::getInstance().getPlayers()[currentPlayerIndex]});
        close();
        return;
    }

    switch (currentPlayerIndex) {
    case 0:
        if(game::gamePlay::GameInstance::getInstance().getPlayers()[0]->isBankrupted()) {
            return;
        }
        scenePlayer1->setBackgroundBrush(Qt::red);
        scenePlayer2->setBackgroundBrush(Qt::transparent);
        scenePlayer3->setBackgroundBrush(Qt::transparent);
        scenePlayer4->setBackgroundBrush(Qt::transparent);
        break;
    case 1:
        if(game::gamePlay::GameInstance::getInstance().getPlayers()[1]->isBankrupted()) {
            return;
        }
        scenePlayer1->setBackgroundBrush(Qt::transparent);
        scenePlayer2->setBackgroundBrush(Qt::cyan);
        scenePlayer3->setBackgroundBrush(Qt::transparent);
        scenePlayer4->setBackgroundBrush(Qt::transparent);
        break;
    case 2:
        if(game::gamePlay::GameInstance::getInstance().getPlayers()[2]->isBankrupted()) {
            return;
        }
        scenePlayer1->setBackgroundBrush(Qt::transparent);
        scenePlayer2->setBackgroundBrush(Qt::transparent);
        scenePlayer3->setBackgroundBrush(Qt::yellow);
        scenePlayer4->setBackgroundBrush(Qt::transparent);
        break;
    case 3:
        if(game::gamePlay::GameInstance::getInstance().getPlayers()[3]->isBankrupted()) {
            return;
        }
        scenePlayer1->setBackgroundBrush(Qt::transparent);
        scenePlayer2->setBackgroundBrush(Qt::transparent);
        scenePlayer3->setBackgroundBrush(Qt::transparent);
        scenePlayer4->setBackgroundBrush(Qt::green);
        break;
    }

    int i = 0;
    for (auto& it : game::gamePlay::GameInstance::getInstance().getPlayers()) {
        QGraphicsColorizeEffect* grayscaleEffect = new QGraphicsColorizeEffect();
        grayscaleEffect->setColor(Qt::gray);
        grayscaleEffect->setStrength(1.0);
        if (it->isBankrupted()) {
            switch(i) {
            case 0:
                ui->playerAvatar_1->setGraphicsEffect(grayscaleEffect);
                break;
            case 1:
                ui->playerAvatar_2->setGraphicsEffect(grayscaleEffect);
                break;
            case 2:
                ui->playerAvatar_3->setGraphicsEffect(grayscaleEffect);
                break;
            case 3:
                ui->playerAvatar_4->setGraphicsEffect(grayscaleEffect);
                break;
            }
        }
        i++;
    }

    if (currentBid + req.bidIncrement > game::gamePlay::GameInstance::getInstance().getPlayers()[currentPlayerIndex]->getCash()) {
        int minimum = std::max(currentBid + req.bidIncrement, req.reservePrice);
        ui->bidInput->setMinimum(minimum);
        ui->bidInput->setMaximum(currentBid + req.bidIncrement);
        ui->bidButton->setDisabled(true);
    }
    else {
        int minimum = std::max(currentBid + req.bidIncrement, req.reservePrice);
        ui->bidInput->setMinimum(minimum);
        ui->bidInput->setMaximum(game::gamePlay::GameInstance::getInstance().getPlayers()[currentPlayerIndex]->getCash());
        ui->bidButton->setDisabled(false);
    }
}

void auctionWidget::on_bidButton_clicked()
{
    if (currentBid + req.bidIncrement <= game::gamePlay::GameInstance::getInstance().getPlayers()[currentPlayerIndex]->getCash()) {
        maxBidPlayerIndex = currentPlayerIndex;
        currentBid = ui->bidInput->value();
        ui->currentBidLabel->setText(QString::fromStdString("Current bid: $" + std::to_string(currentBid) + ", new bid: "));
        nextPlayer();
    }
}

void auctionWidget::on_passButton_clicked()
{
    nextPlayer();
}
