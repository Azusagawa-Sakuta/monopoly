#include "auctionwidget.h"
#include "ui_auctionwidget.h"

auctionWidget::auctionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::auctionWidget)
{
    ui->setupUi(this);
}

auctionWidget::~auctionWidget()
{
    delete ui;
}
