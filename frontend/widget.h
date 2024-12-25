#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsPixmapItem>

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

    void thumbnailDoubleClicked(int index);

private slots:

    void on_manualButton_clicked();

    void on_cancelButton_clicked();

    void on_commitButton_clicked();

    void on_addComputer_1_clicked();

    void on_addPlayer_1_clicked();

    void on_addComputer_2_clicked();

    void on_addPlayer_2_clicked();

    void on_addComputer_3_clicked();

    void on_addPlayer_3_clicked();

    void on_addComputer_4_clicked();

    void on_addPlayer_4_clicked();

    void handleThumbnailDoubleClick(int index);

private:
    Ui::Widget *ui;
    QGraphicsScene *scene;
    QGraphicsScene *scenePlayer1;
    QGraphicsScene *scenePlayer2;
    QGraphicsScene *scenePlayer3;
    QGraphicsScene *scenePlayer4;
    int selectedPlayers;

    void loadPhotos();
    void loadProfiles(int player, int index);
};

class ClickablePixmapItem : public QGraphicsPixmapItem
{
public:
    ClickablePixmapItem(const QPixmap &pixmap, const int index);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int index;
};

#endif // WIDGET_H
