#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>

namespace Ui {
class startWidget;
}

class startWidget : public QWidget
{
    Q_OBJECT

public:
    explicit startWidget(QWidget *parent = nullptr);
    ~startWidget();

private slots:
    void on_startButton_clicked();

    void on_manualButton_clicked();

    void on_closeButton_clicked();

    void on_aboutUsButton_clicked();

    void on_quickStartButton_clicked();

private:
    Ui::startWidget *ui;
};

#endif // STARTWIDGET_H
