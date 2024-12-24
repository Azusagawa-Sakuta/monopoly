#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void playerSelected(int playerCount);

private slots:

    void on_onePlayerButton_toggled(bool checked);

    void on_twoPlayersButton_toggled(bool checked);

    void on_threePlayersButton_toggled(bool checked);

    void on_fourPlayersButton_toggled(bool checked);

    void on_startButton_clicked();

    void on_exitButton_clicked();

    void on_manualButton_clicked();

private:
    Ui::Widget *ui;
    int selectedPlayers;
};
#endif // STARTWIDGET_H
