#include "startwidget.h"
#include "gamemainwidget.h"
#include "ui_startwidget.h"
#include <QMessageBox>
#include <QDialog>
#include <QDesktopServices>
#include "widget.h"

startWidget::startWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::startWidget)
{
    ui->setupUi(this);
    
    showMaximized();
}

startWidget::~startWidget()
{
    delete ui;
}

void startWidget::on_startButton_clicked()
{
    // new widget here
    Widget* w = new Widget;
    w->show();
    this->close();
}

void startWidget::on_quickStartButton_clicked()
{
    QStringList imagePaths = {":/resources/avatar/avatar1.jpg", ":/resources/avatar/avatar2.jpg", ":/resources/avatar/avatar3.jpg", ":/resources/avatar/avatar4.jpg"};
    QStringList finalImagePaths = imagePaths;

    int number = 10;

    auto& g = game::gamePlay::GameInstance::getInstance();

    for (int i = 0; i < 4; i++) {
        g.addPlayer(new game::player::ComputerPlayer);
        // add profile photo through $(finalImagePaths) here
        auto playerList = g.getPlayers();
        playerList.back()->setImagePath((finalImagePaths[i]).toStdString());
    }

    g.addTile(new game::gamePlay::Home);
    for (int i = 2; i < number; i++) 
        g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Prison);
    for (int i = 2; i < number; i++) 
        g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Random);
    for (int i = 2; i < number; i++) 
        g.addTile(new game::gamePlay::Buildable);
    g.addTile(new game::gamePlay::Tax);
    for (int i = 2; i < number; i++) 
        g.addTile(new game::gamePlay::Buildable);

    gameMainWidget *w = new gameMainWidget(nullptr);

    w->show(); // Show the game main widget
    this->close(); // Close the current start widget
}


void startWidget::on_manualButton_clicked()
{
    // find manual.pdf here
    QString pdfFilePath = ":/manual.pdf";

    QUrl pdfUrl = QUrl::fromLocalFile(pdfFilePath);

    if(!QDesktopServices::openUrl(pdfUrl)) {
        QMessageBox::critical(this, "Failed to open", "Failed to open manual page, please check if you are in latest version.");
        qDebug() << "Failed to open manual.";
    }
}


void startWidget::on_closeButton_clicked()
{
    auto ok = QMessageBox::question(this, "Quit Game?", "Would you like to quit the Monopoly now?", QMessageBox::Yes | QMessageBox::No);
    if (ok == QMessageBox::Yes) {
        QApplication::quit();
    }
}


void startWidget::on_aboutUsButton_clicked()
{
    QMessageBox::information(this, "About us", "Code: IcaS66, Azusagawa-Sakuta\n"
                                               "UI Design: IcaS66, Azusagawa-Sakuta\n"
                                               "Test: IcaS66, 155TuT, Eleina, Azusagawa-Sakuta");
}

