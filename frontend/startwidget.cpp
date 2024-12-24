#include "startwidget.h"
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

