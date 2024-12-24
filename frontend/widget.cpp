#include "widget.h"
#include "ui_widget.h"
#include "gamemainwidget.h"
#include "startwidget.h"
#include <QDebug>
#include <QString>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QGraphicsPixmapItem>
#include "../backend/game.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , scene(new QGraphicsScene(this))
    , selectedPlayers(0)
{
    ui->setupUi(this);

    ui->profilePhotoList->setScene(scene);

    // load photos
    loadPhotos();

    ui->commitButton->setEnabled(false);

    connect(this, &Widget::playerSelected, [this](int count) {
        qDebug() << "Player selected: " << count;
        bool ok;
        int number = QInputDialog::getInt(this, "Input the length of map", "Please enter a number range in (4, 10) to generate the map: ", 4, 4, 10, 1, &ok);
        if(ok) {
            auto& g = game::gamePlay::GameInstance::getInstance();

            for (int i = 0; i < count; i++) 
                g.addPlayer(new game::player::Player);

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
        else {
            QMessageBox::information(this, "Input Cancelled", "No number was entered, please try again");
        }
    });
}

Widget::~Widget()
{
    delete ui;
    delete scene;
}

void Widget::loadPhotos() {
    QStringList imagePaths = {"image1.png", "image2.png", "image3.png", "image4.png"};
    // 加载图片缩略图到场景
    // TODO (try parameters)
    int x = 0; // 起始 x 坐标
    int y = 0; // 起始 y 坐标
    const int spacing = 50; // 缩略图间距
    QRectF totalBoundingRect; // 用于计算总的边界矩形

    for (const QString &path : imagePaths) {
        QPixmap pixmap(path);

        // 缩略图大小
        QPixmap thumbnail = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // 创建图形项并添加到场景
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(thumbnail);
        item->setPos(x, y); // 设置位置
        scene->addItem(item);

        // 更新总边界矩形
        QRectF itemRect(item->pos(), QSizeF(thumbnail.width(), thumbnail.height()));
        totalBoundingRect = totalBoundingRect.united(itemRect);

        // 更新下一个图片的 x 坐标
        x += thumbnail.width() + spacing;
    }

    // 调整场景范围为总的边界矩形
    scene->setSceneRect(totalBoundingRect);

    // 将 QGraphicsView 居中显示
    ui->profilePhotoList->centerOn(totalBoundingRect.center());
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


void Widget::on_cancelButton_clicked()
{
    startWidget* w = new startWidget;
    w->show();
    this->close();
}


void Widget::on_commitButton_clicked()
{
    if (selectedPlayers >= 2)
    {
        qDebug() << "Game started with" << selectedPlayers << "players";
    }
    else
    {
        QMessageBox::critical(this, "Failed to start", "Please select the number of players before starting the game!");
        qDebug() << "Please select the number of players before starting the game!";
    }

    emit playerSelected(selectedPlayers);
}

void Widget::on_addComputer_1_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    QString txt = ui->addComputer_1->text();

    if (txt == static_cast<QString>("Add Computer")) {
        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
        g.addPlayer(new game::player::ComputerPlayer);
        ui->addComputer_1->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_1->setText(static_cast<QString>("Choose role again?"));
    }

    else if (txt == static_cast<QString>("Select Profile")) {
        // determine the photo
        // TODO

        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
        ui->addComputer_1->setText(static_cast<QString>("Unready"));
        ui->addPlayer_1->setText(static_cast<QString>("Choose profile again?"));
    }

    else if (txt == static_cast<QString>("Unready")) {
        ui->addComputer_1->setText(static_cast<QString>("Ready!"));
        selectedPlayers--;
        if (selectedPlayers < 2) {
            ui->commitButton->setEnabled(false);
        }
    }

    else if (txt == static_cast<QString>("Ready!")) {
        ui->addComputer_1->setText(static_cast<QString>("Unready"));
        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
    }
}


//Important!! Those graphic views inside each box should be initialized as a question mark icon

void Widget::on_addPlayer_1_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    QString txt = ui->addPlayer_1->text();

    if (txt == static_cast<QString>("Add Player")) {
        g.addPlayer(new game::player::Player);
        ui->addComputer_1->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_1->setText(static_cast<QString>("Choose role again?"));
    }
    else if (txt == static_cast<QString>("Choose role again?")) {
        // delete the last player
        // TODO

        ui->addPlayer_1->setText(static_cast<QString>("Add Player"));
        ui->addComputer_1->setText(static_cast<QString>("Add Computer"));
    }
    else if (txt == static_cast<QString>("Choose profile again?")) {
        // change the graphic view of the player into question mark
        // TODO

        selectedPlayers--;
        if (selectedPlayers < 2) {
            ui->commitButton->setEnabled(false);
        }
        ui->addComputer_1->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_1->setText(static_cast<QString>("Choose role again?"));
    }
}


void Widget::on_addComputer_2_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    QString txt = ui->addComputer_2->text();

    if (txt == static_cast<QString>("Add Computer")) {
        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
        g.addPlayer(new game::player::ComputerPlayer);
        ui->addComputer_2->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_2->setText(static_cast<QString>("Choose role again?"));
    }

    else if (txt == static_cast<QString>("Select Profile")) {
        // determine the photo
        // TODO

        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
        ui->addComputer_2->setText(static_cast<QString>("Unready"));
        ui->addPlayer_2->setText(static_cast<QString>("Choose profile again?"));
    }

    else if (txt == static_cast<QString>("Unready")) {
        ui->addComputer_2->setText(static_cast<QString>("Ready!"));
        selectedPlayers--;
        if (selectedPlayers < 2) {
            ui->commitButton->setEnabled(false);
        }
    }

    else if (txt == static_cast<QString>("Ready!")) {
        ui->addComputer_2->setText(static_cast<QString>("Unready"));
        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
    }
}


void Widget::on_addPlayer_2_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    QString txt = ui->addPlayer_2->text();

    if (txt == static_cast<QString>("Add Player")) {
        g.addPlayer(new game::player::Player);
        ui->addComputer_2->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_2->setText(static_cast<QString>("Choose role again?"));
    }
    else if (txt == static_cast<QString>("Choose role again?")) {
        // delete the last player
        // TODO

        ui->addPlayer_2->setText(static_cast<QString>("Add Player"));
        ui->addComputer_2->setText(static_cast<QString>("Add Computer"));
    }
    else if (txt == static_cast<QString>("Choose profile again?")) {
        // change the graphic view of the player into question mark
        // TODO

        selectedPlayers--;
        if (selectedPlayers < 2) {
            ui->commitButton->setEnabled(false);
        }
        ui->addComputer_2->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_2->setText(static_cast<QString>("Choose role again?"));
    }
}

void Widget::on_addComputer_3_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    QString txt = ui->addComputer_3->text();

    if (txt == static_cast<QString>("Add Computer")) {
        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
        g.addPlayer(new game::player::ComputerPlayer);
        ui->addComputer_3->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_3->setText(static_cast<QString>("Choose role again?"));
    }

    else if (txt == static_cast<QString>("Select Profile")) {
        // determine the photo
        // TODO

        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
        ui->addComputer_3->setText(static_cast<QString>("Unready"));
        ui->addPlayer_3->setText(static_cast<QString>("Choose profile again?"));
    }

    else if (txt == static_cast<QString>("Unready")) {
        ui->addComputer_3->setText(static_cast<QString>("Ready!"));
        selectedPlayers--;
        if (selectedPlayers < 2) {
            ui->commitButton->setEnabled(false);
        }
    }

    else if (txt == static_cast<QString>("Ready!")) {
        ui->addComputer_3->setText(static_cast<QString>("Unready"));
        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
    }
}


void Widget::on_addPlayer_3_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    QString txt = ui->addPlayer_3->text();

    if (txt == static_cast<QString>("Add Player")) {
        g.addPlayer(new game::player::Player);
        ui->addComputer_3->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_3->setText(static_cast<QString>("Choose role again?"));
    }
    else if (txt == static_cast<QString>("Choose role again?")) {
        // delete the last player
        // TODO

        ui->addPlayer_3->setText(static_cast<QString>("Add Player"));
        ui->addComputer_3->setText(static_cast<QString>("Add Computer"));
    }
    else if (txt == static_cast<QString>("Choose profile again?")) {
        // change the graphic view of the player into question mark
        // TODO

        selectedPlayers--;
        if (selectedPlayers < 2) {
            ui->commitButton->setEnabled(false);
        }
        ui->addComputer_3->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_3->setText(static_cast<QString>("Choose role again?"));
    }
}

void Widget::on_addComputer_4_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    QString txt = ui->addComputer_4->text();

    if (txt == static_cast<QString>("Add Computer")) {
        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
        g.addPlayer(new game::player::ComputerPlayer);
        ui->addComputer_4->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_4->setText(static_cast<QString>("Choose role again?"));
    }

    else if (txt == static_cast<QString>("Select Profile")) {
        // determine the photo
        // TODO

        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
        ui->addComputer_4->setText(static_cast<QString>("Unready"));
        ui->addPlayer_4->setText(static_cast<QString>("Choose profile again?"));
    }

    else if (txt == static_cast<QString>("Unready")) {
        ui->addComputer_4->setText(static_cast<QString>("Ready!"));
        selectedPlayers--;
        if (selectedPlayers < 2) {
            ui->commitButton->setEnabled(false);
        }
    }

    else if (txt == static_cast<QString>("Ready!")) {
        ui->addComputer_4->setText(static_cast<QString>("Unready"));
        selectedPlayers++;
        if (selectedPlayers >= 2) {
            ui->commitButton->setEnabled(true);
        }
    }
}


void Widget::on_addPlayer_4_clicked()
{
    auto& g = game::gamePlay::GameInstance::getInstance();
    QString txt = ui->addPlayer_4->text();

    if (txt == static_cast<QString>("Add Player")) {
        g.addPlayer(new game::player::Player);
        ui->addComputer_4->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_4->setText(static_cast<QString>("Choose role again?"));
    }
    else if (txt == static_cast<QString>("Choose role again?")) {
        // delete the last player
        // TODO

        ui->addPlayer_4->setText(static_cast<QString>("Add Player"));
        ui->addComputer_4->setText(static_cast<QString>("Add Computer"));
    }
    else if (txt == static_cast<QString>("Choose profile again?")) {
        // change the graphic view of the player into question mark
        // TODO

        selectedPlayers--;
        if (selectedPlayers < 2) {
            ui->commitButton->setEnabled(false);
        }
        ui->addComputer_4->setText(static_cast<QString>("Select Profile"));
        ui->addPlayer_4->setText(static_cast<QString>("Choose role again?"));
    }
}

