#ifndef AUCTIONWIDGET_H
#define AUCTIONWIDGET_H

#include <QWidget>

namespace Ui {
class auctionWidget;
}

class auctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit auctionWidget(QWidget *parent = nullptr);
    ~auctionWidget();

private:
    Ui::auctionWidget *ui;
};

#endif // AUCTIONWIDGET_H
