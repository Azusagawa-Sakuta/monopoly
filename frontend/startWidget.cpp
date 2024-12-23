#include "startWidget.h"
#include "ui_startWidget.h"
#include "gamemainwidget.h"
#include <QDebug>
#include <QString>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QInputDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , selectedPlayers(0)
{
    ui->setupUi(this);

    connect(this, &Widget::playerSelected, [this](int count) {
        qDebug() << "Player selected: " << count;
        bool ok;
        int number = QInputDialog::getInt(this, "Input the length of map", "Please enter a number range in (4, 10) to generate the map: ", 4, 4, 10, 1, &ok);
        if(ok) {
            gameMainWidget *w = new gameMainWidget(nullptr, number);

            w->show(); // Show the game main widget
            this->close(); // Close the current start widget
        }
        else {
            QMessageBox::information(this, "Input Cancelled", "No number was entered, please try again");
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

// Send a signal for one player
void Widget::on_onePlayerButton_toggled(bool checked)
{
    if (checked) {
        selectedPlayers = 1;
        emit playerSelected(selectedPlayers);
    }
}

// Send a signal for two players
void Widget::on_twoPlayersButton_toggled(bool checked)
{
    if (checked) {
        selectedPlayers = 2;
        emit playerSelected(selectedPlayers);
    }
}

// Send a signal for three players
void Widget::on_threePlayersButton_toggled(bool checked)
{
    if (checked) {
        selectedPlayers = 3;
        emit playerSelected(selectedPlayers);
    }
}

// Send a signal for four players
void Widget::on_fourPlayersButton_toggled(bool checked)
{
    if (checked) {
        selectedPlayers = 4;
        emit playerSelected(selectedPlayers);
    }
}

// Read the signal now and start the game
void Widget::on_startButton_clicked()
{
    if (selectedPlayers > 0)
    {
        qDebug() << "Game started with" << selectedPlayers << "players";
    }
    else
    {
        QMessageBox::critical(this, "Failed to start", "Please select the number of players before starting the game!");
        qDebug() << "Please select the number of players before starting the game!";
    }
}

void Widget::on_exitButton_clicked()
{
    qDebug() << "Game exit";
    QApplication::quit();
}


void Widget::on_manualButton_clicked()
{
    QString pdfFilePath = ":/manual.pdf";

    QUrl pdfUrl = QUrl::fromLocalFile(pdfFilePath);

    if(!QDesktopServices::openUrl(pdfUrl)) {
        QMessageBox::critical(this, "Failed to open", "Failed to open manual page, please check if you are in latest version.");
        qDebug() << "Failed to open manual.";
    }
}

