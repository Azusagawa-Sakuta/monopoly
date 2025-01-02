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

    void initialize();

private:
    Ui::sellWidget *ui;

    void update();

private slots:
    void moveToTreeView2(const QModelIndex &index);
    void moveToTreeView1(const QModelIndex &index);
    void on_sellButton_clicked();
    void on_cancelButton_clicked();
};

#endif // SELLWIDGET_H
