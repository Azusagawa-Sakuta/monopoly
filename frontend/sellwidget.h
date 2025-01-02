#ifndef SELLWIDGET_H
#define SELLWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "../backend/game.h"

namespace Ui {
class sellWidget;
}

class sellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sellWidget(QWidget *parent = nullptr);
    ~sellWidget();

private:
    Ui::sellWidget *ui;
    void initialize();
};

#endif // SELLWIDGET_H
