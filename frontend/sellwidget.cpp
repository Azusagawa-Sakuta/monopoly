#include "sellwidget.h"
#include "ui_sellwidget.h"

sellWidget::sellWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sellWidget)
{
    ui->setupUi(this);
}

sellWidget::~sellWidget()
{
    delete ui;
}

void sellWidget::initialize() {
    auto& gameInstance = game::gamePlay::GameInstance::getInstance();
    auto currentPlayer = gameInstance.getCurrentPlayer();
    auto ownTiles = gameInstance.findOwnTiles(currentPlayer);

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"Tile ID", "Value"});

    for (const auto& tile : ownTiles) {
        auto buildable = static_cast<game::gamePlay::Buildable*>(tile);
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(gameInstance.findTile(tile)));
        row << new QStandardItem(QString::number(buildable->getValue()));
        model->appendRow(row);
    }

    ui->treeView1->setModel(model);
}