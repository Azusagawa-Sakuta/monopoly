#include "auctionwidget.h"
#include "ui_auctionwidget.h"
#include "../backend/game.h"

#include <QGraphicsColorizeEffect>
#include <ranges>

auctionWidget::auctionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::auctionWidget),
    sceneTile(new QGraphicsScene(this))
{
    for (auto& scenePlayer : scenePlayers) {
        scenePlayer = new QGraphicsScene(this);
    }
    ui->setupUi(this);
}

void auctionWidget::initialize(game::gamePlay::Buildable* tile, game::cashType reservePrice, game::cashType bidIncrement) {
    sceneTile->clear();
    for (auto& scenePlayer : scenePlayers) {
        scenePlayer->clear();
    }

    ui->tileGraphics->setScene(sceneTile);

    for (auto&& [playerAvatar, scenePlayer] : std::views::zip(
        ui->playerAvatars,
        scenePlayers | std::views::as_const
    )) {
        playerAvatar->setScene(scenePlayer);
    }

    req = {reservePrice, bidIncrement, tile};

    const auto& g = game::gamePlay::GameInstance::getInstance();
    const auto& playerList = g.getPlayers();

    auto zipped_views = std::views::zip(
        playerList | std::views::as_const,
        scenePlayers | std::views::as_const,
        ui->playerAvatars | std::views::as_const,
        ui->playerLabels | std::views::as_const,
        ui->playerValues | std::views::as_const
    );
    for (auto&& [it, scenePlayer, playerAvatar, playerLabel, playerValue] : zipped_views) {
        const QPixmap pixmap(QString::fromStdString(it->getImagePath()));
        const QPixmap scaledPixmap = pixmap.scaled(512, 512, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(scaledPixmap);
        scenePlayer->addItem(item);
        item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
        playerAvatar->fitInView(scenePlayer->sceneRect(), Qt::KeepAspectRatio);
        playerLabel->setText(QString::fromStdString(it->getNickname()));
        playerValue->setText(QString::fromStdString("Value: $" + std::to_string(it->getCash())));
    }

    QPixmap pixmap(getTileImage(req.tile));
    QPixmap scaledPixmap = pixmap.scaled(256, 256, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(scaledPixmap);

    sceneTile->addItem(item);
    item->setPos(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
    ui->tileGraphics->fitInView(sceneTile->sceneRect(), Qt::KeepAspectRatioByExpanding);

    ui->tileLabel->setText(QString::fromStdString("Tile " + std::to_string(g.findTile(req.tile)) +
        " is being auctioned, with minimum $" + std::to_string(req.reservePrice) +
        " and increment $" + std::to_string(req.bidIncrement) + "."));

    maxBidPlayerIndex = -1;
    currentPlayerIndex = -1;
    currentBid = 500;
    round = 0;

    ui->bidInput->setMinimum(req.reservePrice);
    ui->bidInput->setSingleStep(req.bidIncrement);
    ui->bidInput->setMaximum(114514);
    ui->bidInput->setValue(req.reservePrice);

    ui->currentBidLabel->setText(QString::fromStdString("Current bid: $" + std::to_string(req.reservePrice) + ", new bid: "));

    nextPlayer();
}

auctionWidget::~auctionWidget()
{
    delete ui;
}

QPixmap auctionWidget::getTileImage(game::gamePlay::Tile* tile)
{
    QPixmap tileImage;
    switch (tile->getType()) {
    using game::gamePlay::Tile;
    case Tile::TileType::buildable: {
        auto const buildableTile = static_cast<const game::gamePlay::Buildable*>(tile);
        std::string colors[] = {
            ":/resources/tile/red.png",
            ":/resources/tile/green.png",
            ":/resources/tile/blue.png",
            ":/resources/tile/yellow.png"
        };
        tileImage = QPixmap(QString::fromStdString(colors[buildableTile->getColor()]));
        break;
    }
    case Tile::TileType::home:
        tileImage = QPixmap(":/resources/tile/go.png");
        break;
    case Tile::TileType::random:
        tileImage = QPixmap(":/resources/tile/casino.png");
        break;
    case Tile::TileType::tax:
        tileImage = QPixmap(":/resources/tile/ccf.png");
        break;
    case Tile::TileType::prison:
        tileImage = QPixmap(":/resources/tile/prison.png");
        break;
    default:
        qDebug() << "Failed to load image";
    }
    return tileImage;
}

void auctionWidget::nextPlayer() {
    auto& g = game::gamePlay::GameInstance::getInstance();
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % g.getPlayers().size();
        round++;
    }
    while (g.getPlayers()[currentPlayerIndex]->isBankrupted());

    // if (g.getPlayers()[currentPlayerIndex]->isBankrupted()) {
    //     nextPlayer();
    // }

    for (const auto& scenePlayer : scenePlayers) {
        scenePlayer->setBackgroundBrush(Qt::transparent);
    }

    const std::array<const Qt::GlobalColor, 4> BackgroundColors = {
        Qt::red,
        Qt::yellow,
        Qt::cyan,
        Qt::green
    };
    scenePlayers[currentPlayerIndex]->setBackgroundBrush(
        BackgroundColors[currentPlayerIndex]
    );


    for (auto&& [it, playerAvatar] : std::views::zip(g.getPlayers(), ui->playerAvatars)) {
        QGraphicsColorizeEffect* grayscaleEffect = new QGraphicsColorizeEffect();
        grayscaleEffect->setColor(Qt::gray);
        grayscaleEffect->setStrength(1.0);
        if (it->isBankrupted()) {
            playerAvatar->setGraphicsEffect(grayscaleEffect);
        }
    }

    if (round == static_cast<int>(g.getPlayers().size()) + 1 && maxBidPlayerIndex == -1) {
        QMessageBox::information(this, "Auction Ended", QString::fromStdString("No one bid for tile " + std::to_string(g.findTile(req.tile)) + "."));
        g.provideInput(game::gamePlay::GameInstance::auctionResult{0, nullptr});
        this->hide();
        return;
    }

    if (maxBidPlayerIndex == currentPlayerIndex) {
        QMessageBox::information(this, "Auction Ended", QString::fromStdString(g.getPlayers()[maxBidPlayerIndex]->getNickname() + " won the auction for $" + std::to_string(currentBid) + "."));
        g.provideInput(game::gamePlay::GameInstance::auctionResult{currentBid, g.getPlayers()[currentPlayerIndex]});
        this->hide();
        return;
    }

    if (g.getPlayers()[currentPlayerIndex]->isComputer())
        nextPlayer();

    int minimum = std::max(currentBid + req.bidIncrement, req.reservePrice);
    ui->bidInput->setMinimum(minimum);
    ui->bidInput->setValue(minimum);

    if (currentBid + req.bidIncrement > g.getPlayers()[currentPlayerIndex]->getCash()) {
        ui->bidInput->setMaximum(minimum);
        ui->bidButton->setDisabled(true);
    }
    else {
        ui->bidInput->setMaximum((g.getPlayers()[currentPlayerIndex]->getCash()) / 500 * 500);
        ui->bidButton->setDisabled(false);
    }
}

void auctionWidget::on_bidButton_clicked()
{
    assert(currentBid + req.bidIncrement <= game::gamePlay::GameInstance::getInstance().getPlayers()[currentPlayerIndex]->getCash());
    maxBidPlayerIndex = currentPlayerIndex;
    currentBid = ui->bidInput->value();
    ui->currentBidLabel->setText(QString::fromStdString("Current bid: $" + std::to_string(currentBid) + ", new bid: "));
    nextPlayer();
}

void auctionWidget::on_passButton_clicked()
{
    nextPlayer();
}
