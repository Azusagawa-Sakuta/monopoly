#include "sellwidget.h"
#include "ui_sellwidget.h"

sellWidget::sellWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sellWidget)
{
    ui->setupUi(this);

    connect(ui->treeView1, &QTreeView::clicked, this, &sellWidget::moveToTreeView2);
    connect(ui->treeView2, &QTreeView::clicked, this, &sellWidget::moveToTreeView1);
}

sellWidget::~sellWidget()
{
    delete ui;
}

void sellWidget::initialize() {
    auto& gameInstance = game::gamePlay::GameInstance::getInstance();
    auto currentPlayer = gameInstance.getCurrentPlayer();
    game::cashType cash = currentPlayer->getCash();
    auto rent = std::any_cast<game::cashType>(gameInstance.getActiveEventParam());
    auto remaining = rent - cash;

    ui->labelIntro->setText(QString("You have %1$, but have to pay %2$. Still need to pay %3$.")
                            .arg(cash).arg(rent).arg(remaining));

    auto ownTiles = gameInstance.findOwnTiles(currentPlayer);

    QStandardItemModel *model1 = new QStandardItemModel(this);
    model1->setHorizontalHeaderLabels({"Tile ID", "Value"});

    for (const auto& tile : ownTiles) {
        auto buildable = static_cast<game::gamePlay::Buildable*>(tile);
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(gameInstance.findTile(tile)));
        row << new QStandardItem(QString::number(buildable->getValue()));
        model1->appendRow(row);
    }

    ui->treeView1->setModel(model1);

    QStandardItemModel *model2 = new QStandardItemModel(this);
    model2->setHorizontalHeaderLabels({"Tile ID", "Value"});
    ui->treeView2->setModel(model2);

    update();
}

void sellWidget::moveToTreeView2(const QModelIndex &index) {
    QStandardItemModel *model1 = static_cast<QStandardItemModel*>(ui->treeView1->model());
    QStandardItemModel *model2 = static_cast<QStandardItemModel*>(ui->treeView2->model());

    QList<QStandardItem*> row;
    for (int i = 0; i < model1->columnCount(); ++i) {
        row.append(model1->itemFromIndex(model1->index(index.row(), i))->clone());
    }
    model2->appendRow(row);
    model1->removeRow(index.row());

    update();
}

void sellWidget::moveToTreeView1(const QModelIndex &index) {
    QStandardItemModel *model1 = static_cast<QStandardItemModel*>(ui->treeView1->model());
    QStandardItemModel *model2 = static_cast<QStandardItemModel*>(ui->treeView2->model());

    QList<QStandardItem*> row;
    for (int i = 0; i < model2->columnCount(); ++i) {
        row.append(model2->itemFromIndex(model2->index(index.row(), i))->clone());
    }
    model1->appendRow(row);
    model2->removeRow(index.row());

    update();
}

void sellWidget::on_sellButton_clicked() {
    auto& gameInstance = game::gamePlay::GameInstance::getInstance();
    auto currentPlayer = gameInstance.getCurrentPlayer();
    auto ownTiles = gameInstance.findOwnTiles(currentPlayer);

    QStandardItemModel *model2 = static_cast<QStandardItemModel*>(ui->treeView2->model());
    std::vector<game::gamePlay::Buildable*> toSell;
    for (int i = 0; i < model2->rowCount(); ++i) {
        int tileIndex = std::stoi(model2->item(i, 0)->text().toStdString());
        auto it = std::find_if(ownTiles.begin(), ownTiles.end(), [&gameInstance, tileIndex](game::gamePlay::Tile* tile) {
            return gameInstance.findTile(tile) == tileIndex;
        });
        if (it != ownTiles.end()) {
            toSell.push_back(static_cast<game::gamePlay::Buildable*>(*it));
        } else {
            qDebug() << "Invalid tile index: " << tileIndex;
        }
    }

    gameInstance.notifyUserInput(toSell);
    this->close();
}

void sellWidget::on_cancelButton_clicked() {
    std::vector<game::gamePlay::Buildable*> toSell;
    game::gamePlay::GameInstance::getInstance().notifyUserInput(toSell);
    this->close();
}

void sellWidget::update() {
    auto& gameInstance = game::gamePlay::GameInstance::getInstance();
    auto currentPlayer = gameInstance.getCurrentPlayer();
    game::cashType cash = currentPlayer->getCash();
    auto rent = std::any_cast<game::cashType>(gameInstance.getActiveEventParam());

    QStandardItemModel *model2 = static_cast<QStandardItemModel*>(ui->treeView2->model());
    int totalValue = 0;
    for (int i = 0; i < model2->rowCount(); ++i) {
        totalValue += model2->item(i, 1)->text().toInt();
    }

    int sellValue = totalValue / 2;
    ui->label2->setText(QString("Total Value: %1$").arg(sellValue));

    if (sellValue + cash >= rent) {
        ui->sellButton->setEnabled(true);
    } else {
        ui->sellButton->setEnabled(false);
    }
}
