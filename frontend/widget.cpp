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
    , scenePlayer1(new QGraphicsScene(this))
    , scenePlayer2(new QGraphicsScene(this))
    , scenePlayer3(new QGraphicsScene(this))
    , scenePlayer4(new QGraphicsScene(this))
    , selectedPlayers(0)
{
    ui->setupUi(this);
    
    showMaximized();

    ui->profilePhotoList->setScene(scene);

    ui->playerProfilePhoto_1->setScene(scenePlayer1);
    ui->playerProfilePhoto_2->setScene(scenePlayer2);
    ui->playerProfilePhoto_3->setScene(scenePlayer3);
    ui->playerProfilePhoto_4->setScene(scenePlayer4);

    ui->playerProfilePhoto_1->hide();
    ui->playerProfilePhoto_2->hide();
    ui->playerProfilePhoto_3->hide();
    ui->playerProfilePhoto_4->hide();

    ui->commitButton->setEnabled(false);

    imagePaths = {":/resources/use.png", ":/resources/absolute.png", ":/resources/path.png", ":/resources/here.png"};
    finalImagePaths = imagePaths;

    connect(this, &Widget::playerSelected, [this](int count) {
        qDebug() << "Player selected: " << count;
        bool ok;
        int number = QInputDialog::getInt(this, "Input the length of map", "Please enter a number range in (4, 10) to generate the map: ", 4, 4, 10, 1, &ok);
        if(ok) {
            auto& g = game::gamePlay::GameInstance::getInstance();

            int i = 0;
            for (const auto& item : player) {
                if (item == 1) {
                    g.addPlayer(new game::player::Player);
                    // add profile photo through $(finalImagePaths) here
                    auto playerList = g.getPlayers();
                    playerList.back()->setImagePath((finalImagePaths[i]).toStdString());
                }
                else {
                    g.addPlayer(new game::player::ComputerPlayer);
                    // add profile photo through $(finalImagePaths) here
                    auto playerList = g.getPlayers();
                    playerList.back()->setImagePath((finalImagePaths[i]).toStdString());
                }
                ++i;
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
        else {
            QMessageBox::information(this, "Input Cancelled", "No number was entered, please try again");
        }
    });

    connect(this, &Widget::thumbnailDoubleClicked, this, &Widget::handleThumbnailDoubleClick);

    loadPhotos();
}

Widget::~Widget()
{
    delete ui;
    delete scene;
}

void Widget::loadPhotos() {
    // 加载图片缩略图到场景
    // TODO (try parameters)
    // ---FINISHED---
    int x = 0; // 起始 x 坐标
    int y = 0; // 起始 y 坐标
    const int spacing = 50; // 缩略图间距
    QRectF totalBoundingRect; // 用于计算总的边界矩形
    int index = 0;

    for (const QString &path : imagePaths) {
        QPixmap pixmap(path);

        // 缩略图大小
        QPixmap thumbnail = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // 创建图形项并添加到场景
        ClickablePixmapItem *item = new ClickablePixmapItem(thumbnail, index++);
        item->setPos(x, y); // 设置位置
        scene->addItem(item);

        // 更新总边界矩形
        QRectF itemRect(item->pos(), QSizeF(thumbnail.width(), thumbnail.height()));
        totalBoundingRect = totalBoundingRect.united(itemRect);

        // 更新下一个图片的 x 坐标
        x += thumbnail.width() + spacing;

        item->setFlag(QGraphicsItem::ItemIsSelectable);  // 允许选择项
        item->setFlag(QGraphicsItem::ItemIsFocusable);    // 允许焦点
    }

    // 调整场景范围为总的边界矩形
    scene->setSceneRect(totalBoundingRect);

    // 将 QGraphicsView 居中显示
    ui->profilePhotoList->centerOn(totalBoundingRect.center());

    ui->profilePhotoList->hide();
}

void Widget::loadProfiles(int player, int index) {
    finalImagePaths[player - 1] = imagePaths[index];
    QPixmap pixmap(imagePaths[index]);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    switch (player) {
    case 1:
        scenePlayer1->clear();
        scenePlayer1->addItem(item);
        ui->playerProfilePhoto_1->setScene(scenePlayer1);
        ui->playerProfilePhoto_1->show();
        break;
    case 2:
        scenePlayer2->clear();
        scenePlayer2->addItem(item);
        ui->playerProfilePhoto_2->setScene(scenePlayer2);
        ui->playerProfilePhoto_2->show();
        break;
    case 3:
        scenePlayer3->clear();
        scenePlayer3->addItem(item);
        ui->playerProfilePhoto_3->setScene(scenePlayer3);
        ui->playerProfilePhoto_3->show();
        break;
    case 4:
        scenePlayer4->clear();
        scenePlayer4->addItem(item);
        ui->playerProfilePhoto_4->setScene(scenePlayer4);
        ui->playerProfilePhoto_4->show();
        break;
    }
}

void Widget::handleThumbnailDoubleClick(int index) {
    // IMPORTANT!!
    // Use the double click function here
    loadProfiles(selectedPlayers + 1, index);
    qDebug() << "Thumbnail double clicked: " << index;
}

ClickablePixmapItem::ClickablePixmapItem(const QPixmap &pixmap, const int index)
    : QGraphicsPixmapItem(pixmap), index(index) {
    setAcceptHoverEvents(true);
}

void ClickablePixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPixmapItem::mouseDoubleClickEvent(event);
    emit static_cast<Widget*>(scene()->parent())->thumbnailDoubleClicked(index);
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
    qDebug() << "Game started with" << selectedPlayers << "players";
    emit playerSelected(selectedPlayers);
}

void Widget::on_addComputer_1_clicked()
{
    QString txt = ui->addComputer_1->text();

    if (txt == static_cast<QString>("Add Computer")) {
        if (selectedPlayers != 0) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        player.append(0);
        ui->addComputer_1->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_1->setText(static_cast<QString>("Choose role again?"));
    }

    else if (txt == static_cast<QString>("Select Profile (double-click)")) {
        // determine the photo
        // TODO
        // --OK--
        if (selectedPlayers != 0) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        ui->profilePhotoList->show();
        ui->addComputer_1->setText(static_cast<QString>("Ready!"));
        ui->addPlayer_1->setText(static_cast<QString>("Choose profile again?"));
    }

    else if (txt == static_cast<QString>("Unready")) {
        if (selectedPlayers != 1) {
            QMessageBox::critical(this, "Unready failed", "You should cancel all candidates at the right first.");
            return;
        }
        ui->addComputer_1->setText(static_cast<QString>("Ready!"));
        selectedPlayers--;
    }

    else if (txt == static_cast<QString>("Ready!")) {
        if (selectedPlayers != 0) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        if (!ui->playerProfilePhoto_1->isVisible()) {
            QMessageBox::critical(this, "Ready Failure", "You should not ready until choose your profile photo.");
            return;
        }
        ui->addComputer_1->setText(static_cast<QString>("Unready"));
        ui->profilePhotoList->hide();
        selectedPlayers++;
    }
}


//Important!! Those graphic views inside each box should be initialized as a question mark icon

void Widget::on_addPlayer_1_clicked()
{
    QString txt = ui->addPlayer_1->text();

    if (txt == static_cast<QString>("Add Player")) {
        if (selectedPlayers != 0) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        player.append(1);
        ui->addComputer_1->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_1->setText(static_cast<QString>("Choose role again?"));
    }
    else if (txt == static_cast<QString>("Choose role again?")) {
        // delete the last player
        // TODO
        // --OK--
        player.pop_back();
        if (selectedPlayers != 0) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        ui->addPlayer_1->setText(static_cast<QString>("Add Player"));
        ui->addComputer_1->setText(static_cast<QString>("Add Computer"));
    }
    else if (txt == static_cast<QString>("Choose profile again?")) {
        // change the graphic view of the player into question mark
        // TODO

        if (selectedPlayers == 0) {
            ui->profilePhotoList->hide();
        }
        if (!ui->playerProfilePhoto_2->isVisible() && selectedPlayers == 1) {
            selectedPlayers--;
        }
        if (!ui->playerProfilePhoto_1->isVisible()) {
            QMessageBox::critical(this, "Re-choose failed", "You should cancel all candidates at the right first.");
            return;
        }
        ui->playerProfilePhoto_1->hide();
        ui->addComputer_1->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_1->setText(static_cast<QString>("Choose role again?"));
    }
}


void Widget::on_addComputer_2_clicked()
{
    QString txt = ui->addComputer_2->text();

    if (txt == static_cast<QString>("Add Computer")) {
        if (selectedPlayers != 1) {
            QMessageBox::critical(this, "Add failed", "You should not add another role until all left candidates are determined.");
            return;
        }
        player.append(0);
        ui->addComputer_2->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_2->setText(static_cast<QString>("Choose role again?"));
    }

    else if (txt == static_cast<QString>("Select Profile (double-click)")) {
        // determine the photo
        // TODO
        // --OK--
        if (selectedPlayers != 1) {
            QMessageBox::critical(this, "Select failed", "You should not select a profile photo until all left candidates are determined.");
            return;
        }
        ui->profilePhotoList->show();
        ui->commitButton->setEnabled(true);
        ui->addComputer_2->setText(static_cast<QString>("Ready!"));
        ui->addPlayer_2->setText(static_cast<QString>("Choose profile again?"));
    }

    else if (txt == static_cast<QString>("Unready")) {
        if (selectedPlayers != 2) {
            QMessageBox::critical(this, "Unready failed", "You should cancel all the candidates at the right first.");
            return;
        }
        ui->addComputer_2->setText(static_cast<QString>("Ready!"));
        selectedPlayers--;
        ui->commitButton->setEnabled(false);
    }

    else if (txt == static_cast<QString>("Ready!")) {
        if (selectedPlayers != 1) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        if (!ui->playerProfilePhoto_2->isVisible()) {
            QMessageBox::critical(this, "Ready Failure", "You should not ready until choose your profile photo.");
            return;
        }
        ui->addComputer_2->setText(static_cast<QString>("Unready"));
        ui->profilePhotoList->hide();
        selectedPlayers++;
        ui->commitButton->setEnabled(true);
    }
}


void Widget::on_addPlayer_2_clicked()
{
    QString txt = ui->addPlayer_2->text();

    if (txt == static_cast<QString>("Add Player")) {
        if (selectedPlayers != 1) {
            QMessageBox::critical(this, "Add failed", "You should not add another role until all left candidates are determined.");
            return;
        }
        player.append(1);
        ui->addComputer_2->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_2->setText(static_cast<QString>("Choose role again?"));
    }
    else if (txt == static_cast<QString>("Choose role again?")) {
        // delete the last player
        // TODO
        player.pop_back();
        if (selectedPlayers > 1) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        ui->addPlayer_2->setText(static_cast<QString>("Add Player"));
        ui->addComputer_2->setText(static_cast<QString>("Add Computer"));
    }
    else if (txt == static_cast<QString>("Choose profile again?")) {
        // change the graphic view of the player into question mark
        // TODO

        if (selectedPlayers == 1) {
            ui->profilePhotoList->hide();
        }
        if (!ui->playerProfilePhoto_3->isVisible() && selectedPlayers == 2) {
            selectedPlayers--;
        }
        if (!ui->playerProfilePhoto_2->isVisible()) {
            QMessageBox::critical(this, "Re-choose failed", "You should cancel all the candidates at the right first.");
            return;
        }
        ui->playerProfilePhoto_2->hide();
        ui->commitButton->setEnabled(false);
        ui->addComputer_2->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_2->setText(static_cast<QString>("Choose role again?"));
    }
}

void Widget::on_addComputer_3_clicked()
{
    QString txt = ui->addComputer_3->text();

    if (txt == static_cast<QString>("Add Computer")) {
        if (selectedPlayers != 2) {
            QMessageBox::critical(this, "Add failed", "You should not add another role until all left candidates are determined.");
            return;
        }
        player.append(0);
        ui->addComputer_3->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_3->setText(static_cast<QString>("Choose role again?"));
    }

    else if (txt == static_cast<QString>("Select Profile (double-click)")) {
        // determine the photo
        // TODO
        // --OK--
        if (selectedPlayers != 2) {
            QMessageBox::critical(this, "Select failed", "You should not select a profile photo until all left candidates are determined.");
            return;
        }
        ui->profilePhotoList->show();
        ui->addComputer_3->setText(static_cast<QString>("Ready!"));
        ui->addPlayer_3->setText(static_cast<QString>("Choose profile again?"));
    }

    else if (txt == static_cast<QString>("Unready")) {
        if (selectedPlayers != 3) {
            QMessageBox::critical(this, "Unready failed", "You should cancel all the candidates at the right first.");
            return;
        }
        ui->addComputer_3->setText(static_cast<QString>("Ready!"));
        selectedPlayers--;
    }

    else if (txt == static_cast<QString>("Ready!")) {
        if (selectedPlayers != 2) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        if (!ui->playerProfilePhoto_3->isVisible()) {
            QMessageBox::critical(this, "Ready Failure", "You should not ready until choose your profile photo.");
            return;
        }
        ui->addComputer_3->setText(static_cast<QString>("Unready"));
        ui->profilePhotoList->hide();
        selectedPlayers++;
    }
}


void Widget::on_addPlayer_3_clicked()
{
    QString txt = ui->addPlayer_3->text();

    if (txt == static_cast<QString>("Add Player")) {
        if (selectedPlayers != 2) {
            QMessageBox::critical(this, "Add failed", "You should not add another role until all left candidates are determined.");
            return;
        }
        player.append(1);
        ui->addComputer_3->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_3->setText(static_cast<QString>("Choose role again?"));
    }
    else if (txt == static_cast<QString>("Choose role again?")) {
        // delete the last player
        // TODO
        player.pop_back();
        if (selectedPlayers > 2) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        ui->addPlayer_3->setText(static_cast<QString>("Add Player"));
        ui->addComputer_3->setText(static_cast<QString>("Add Computer"));
    }
    else if (txt == static_cast<QString>("Choose profile again?")) {
        // change the graphic view of the player into question mark
        // TODO

        if (selectedPlayers == 2) {
            ui->profilePhotoList->hide();
        }
        if (!ui->playerProfilePhoto_4->isVisible() && selectedPlayers == 3) {
            selectedPlayers--;
        }
        if (!ui->playerProfilePhoto_3->isVisible()) {
            QMessageBox::critical(this, "Re-choose failed", "You should cancel all the candidates at the right first.");
            return;
        }
        ui->playerProfilePhoto_3->hide();
        ui->addComputer_3->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_3->setText(static_cast<QString>("Choose role again?"));
    }
}

void Widget::on_addComputer_4_clicked()
{
    QString txt = ui->addComputer_4->text();

    if (txt == static_cast<QString>("Add Computer")) {
        if (selectedPlayers != 3) {
            QMessageBox::critical(this, "Add failed", "You should not add another role until all left candidates are determined.");
            return;
        }
        player.append(0);
        ui->addComputer_4->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_4->setText(static_cast<QString>("Choose role again?"));
    }

    else if (txt == static_cast<QString>("Select Profile (double-click)")) {
        // determine the photo
        // TODO
        // --OK--
        if (selectedPlayers != 3) {
            QMessageBox::critical(this, "Select failed", "You should not select a profile photo until all left candidates are determined.");
            return;
        }
        ui->profilePhotoList->show();
        ui->addComputer_4->setText(static_cast<QString>("Ready!"));
        ui->addPlayer_4->setText(static_cast<QString>("Choose profile again?"));
    }

    else if (txt == static_cast<QString>("Unready")) {
        if (selectedPlayers != 4) {
            QMessageBox::critical(this, "Unready failed", "You should cancel all the candidates at the right first.");
            return;
        }
        ui->addComputer_4->setText(static_cast<QString>("Ready!"));
        selectedPlayers--;
    }

    else if (txt == static_cast<QString>("Ready!")) {
        if (selectedPlayers != 3) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        if (!ui->playerProfilePhoto_4->isVisible()) {
            QMessageBox::critical(this, "Ready Failure", "You should not ready until choose your profile photo.");
            return;
        }
        ui->addComputer_4->setText(static_cast<QString>("Unready"));
        ui->profilePhotoList->hide();
        selectedPlayers++;
    }
}


void Widget::on_addPlayer_4_clicked()
{
    QString txt = ui->addPlayer_4->text();

    if (txt == static_cast<QString>("Add Player")) {
        if (selectedPlayers != 3) {
            QMessageBox::critical(this, "Add failed", "You should not add another role until all left candidates are determined.");
            return;
        }
        player.append(1);
        ui->addComputer_4->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_4->setText(static_cast<QString>("Choose role again?"));
    }
    else if (txt == static_cast<QString>("Choose role again?")) {
        // delete the last player
        // TODO
        player.pop_back();
        if (selectedPlayers > 3) {
            QMessageBox::critical(this, "Unexpected Failure", "Don't run program which is modified improperly.");
            return;
        }
        ui->addPlayer_4->setText(static_cast<QString>("Add Player"));
        ui->addComputer_4->setText(static_cast<QString>("Add Computer"));
    }
    else if (txt == static_cast<QString>("Choose profile again?")) {
        // change the graphic view of the player into question mark
        // TODO

        if (selectedPlayers == 3) {
            ui->profilePhotoList->hide();
        }
        if (selectedPlayers == 4) {
            selectedPlayers--;
        }
        if (!ui->playerProfilePhoto_4->isVisible()) {
            QMessageBox::critical(this, "Re-choose failed", "You should cancel all the candidates at the right first.");
            return;
        }
        ui->playerProfilePhoto_4->hide();
        ui->addComputer_4->setText(static_cast<QString>("Select Profile (double-click)"));
        ui->addPlayer_4->setText(static_cast<QString>("Choose role again?"));
    }
}

