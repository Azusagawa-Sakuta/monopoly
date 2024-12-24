/********************************************************************************
** Form generated from reading UI file 'startWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTWIDGET_H
#define UI_STARTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *mainLayout;
    QLabel *gameTitleLabel;
    QVBoxLayout *playerInterface;
    QRadioButton *onePlayerButton;
    QRadioButton *twoPlayersButton;
    QRadioButton *threePlayersButton;
    QRadioButton *fourPlayersButton;
    QHBoxLayout *buttonLayout;
    QSpacerItem *hS;
    QPushButton *startButton;
    QPushButton *exitButton;
    QPushButton *manualButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(1920, 860);
        QFont font;
        font.setFamilies({QString::fromUtf8("Script MT")});
        font.setPointSize(14);
        Widget->setFont(font);
        verticalLayoutWidget_2 = new QWidget(Widget);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(530, 180, 491, 371));
        mainLayout = new QVBoxLayout(verticalLayoutWidget_2);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        gameTitleLabel = new QLabel(verticalLayoutWidget_2);
        gameTitleLabel->setObjectName("gameTitleLabel");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Vivaldi")});
        font1.setPointSize(14);
        font1.setItalic(true);
        gameTitleLabel->setFont(font1);
        gameTitleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainLayout->addWidget(gameTitleLabel);

        playerInterface = new QVBoxLayout();
        playerInterface->setObjectName("playerInterface");
        onePlayerButton = new QRadioButton(verticalLayoutWidget_2);
        onePlayerButton->setObjectName("onePlayerButton");
        QFont font2;
        font2.setFamilies({QString::fromUtf8("STSong")});
        font2.setPointSize(14);
        onePlayerButton->setFont(font2);

        playerInterface->addWidget(onePlayerButton);

        twoPlayersButton = new QRadioButton(verticalLayoutWidget_2);
        twoPlayersButton->setObjectName("twoPlayersButton");
        twoPlayersButton->setFont(font2);

        playerInterface->addWidget(twoPlayersButton);

        threePlayersButton = new QRadioButton(verticalLayoutWidget_2);
        threePlayersButton->setObjectName("threePlayersButton");
        threePlayersButton->setFont(font2);

        playerInterface->addWidget(threePlayersButton);

        fourPlayersButton = new QRadioButton(verticalLayoutWidget_2);
        fourPlayersButton->setObjectName("fourPlayersButton");
        fourPlayersButton->setFont(font2);

        playerInterface->addWidget(fourPlayersButton);


        mainLayout->addLayout(playerInterface);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        hS = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(hS);

        startButton = new QPushButton(verticalLayoutWidget_2);
        startButton->setObjectName("startButton");
        startButton->setFont(font);

        buttonLayout->addWidget(startButton);

        exitButton = new QPushButton(verticalLayoutWidget_2);
        exitButton->setObjectName("exitButton");
        exitButton->setFont(font);

        buttonLayout->addWidget(exitButton);


        mainLayout->addLayout(buttonLayout);

        mainLayout->setStretch(0, 2);
        mainLayout->setStretch(1, 4);
        mainLayout->setStretch(2, 1);
        manualButton = new QPushButton(Widget);
        manualButton->setObjectName("manualButton");
        manualButton->setGeometry(QRect(10, 10, 111, 41));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Script MT")});
        font3.setPointSize(16);
        manualButton->setFont(font3);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        gameTitleLabel->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\"><span style=\" font-size:36pt; font-weight:700;\">Monopoly</span></p></body></html>", nullptr));
        onePlayerButton->setText(QCoreApplication::translate("Widget", "1 Player (with robot\357\274\211", nullptr));
        twoPlayersButton->setText(QCoreApplication::translate("Widget", "2 Players", nullptr));
        threePlayersButton->setText(QCoreApplication::translate("Widget", "3 Players", nullptr));
        fourPlayersButton->setText(QCoreApplication::translate("Widget", "4 Players", nullptr));
        startButton->setText(QCoreApplication::translate("Widget", "Start!", nullptr));
        exitButton->setText(QCoreApplication::translate("Widget", "Close", nullptr));
        manualButton->setText(QCoreApplication::translate("Widget", "Manual", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTWIDGET_H
