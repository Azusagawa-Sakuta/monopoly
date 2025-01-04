/********************************************************************************
** Form generated from reading UI file 'gamemainwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEMAINWIDGET_H
#define UI_GAMEMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gameMainWidget
{
public:
    QGraphicsView *mapView;
    QGridLayout *mainLayout;
    QVBoxLayout *playerLayout_4;
    QLabel *playerNickname_4;
    QHBoxLayout *playerMainLayout_4;
    QGraphicsView *playerAvatarGraphics_4;
    QVBoxLayout *playerInfoLayout_4;
    QLabel *playerInfo_4_1;
    QLabel *playerInfo_4_2;
    QLabel *playerInfo_4_3;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *playerLayout_2;
    QLabel *playerNickname_2;
    QHBoxLayout *playerMainLayout_2;
    QGraphicsView *playerAvatarGraphics_2;
    QVBoxLayout *playerInfoLayout_2;
    QLabel *playerInfo_2_1;
    QLabel *playerInfo_2_2;
    QLabel *playerInfo_2_3;
    QVBoxLayout *playerLayout_1;
    QLabel *playerNickname_1;
    QHBoxLayout *playerMainLayout_1;
    QGraphicsView *playerAvatarGraphics_1;
    QVBoxLayout *playerInfoLayout_1;
    QLabel *playerInfo_1_1;
    QLabel *playerInfo_1_2;
    QLabel *playerInfo_1_3;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QLabel *diceLabel_1;
    QLabel *diceLabel_2;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *rollDiceButton;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *playerLayout_3;
    QLabel *playerNickname_3;
    QHBoxLayout *playerMainLayout_3;
    QGraphicsView *playerAvatarGraphics_3;
    QVBoxLayout *playerInfoLayout_3;
    QLabel *playerInfo_3_1;
    QLabel *playerInfo_3_2;
    QLabel *playerInfo_3_3;

    void setupUi(QWidget *gameMainWidget)
    {
        if (gameMainWidget->objectName().isEmpty())
            gameMainWidget->setObjectName("gameMainWidget");
        gameMainWidget->resize(1920, 1080);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gameMainWidget->sizePolicy().hasHeightForWidth());
        gameMainWidget->setSizePolicy(sizePolicy);
        gameMainWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        gameMainWidget->setWindowOpacity(3.000000000000000);
        mapView = new QGraphicsView(gameMainWidget);
        mapView->setObjectName("mapView");
        mapView->setEnabled(false);
        mapView->setGeometry(QRect(60, 30, 256, 192));
        sizePolicy.setHeightForWidth(mapView->sizePolicy().hasHeightForWidth());
        mapView->setSizePolicy(sizePolicy);
        mapView->setMinimumSize(QSize(0, 0));
        mapView->setMaximumSize(QSize(16777215, 16777215));
        mapView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        mapView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        mapView->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        mapView->setDragMode(QGraphicsView::DragMode::NoDrag);
        mainLayout = new QGridLayout(gameMainWidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        playerLayout_4 = new QVBoxLayout();
        playerLayout_4->setObjectName("playerLayout_4");
        playerNickname_4 = new QLabel(gameMainWidget);
        playerNickname_4->setObjectName("playerNickname_4");
        QFont font;
        font.setPointSize(16);
        playerNickname_4->setFont(font);

        playerLayout_4->addWidget(playerNickname_4);

        playerMainLayout_4 = new QHBoxLayout();
        playerMainLayout_4->setObjectName("playerMainLayout_4");
        playerAvatarGraphics_4 = new QGraphicsView(gameMainWidget);
        playerAvatarGraphics_4->setObjectName("playerAvatarGraphics_4");

        playerMainLayout_4->addWidget(playerAvatarGraphics_4);

        playerInfoLayout_4 = new QVBoxLayout();
        playerInfoLayout_4->setObjectName("playerInfoLayout_4");
        playerInfo_4_1 = new QLabel(gameMainWidget);
        playerInfo_4_1->setObjectName("playerInfo_4_1");

        playerInfoLayout_4->addWidget(playerInfo_4_1);

        playerInfo_4_2 = new QLabel(gameMainWidget);
        playerInfo_4_2->setObjectName("playerInfo_4_2");

        playerInfoLayout_4->addWidget(playerInfo_4_2);

        playerInfo_4_3 = new QLabel(gameMainWidget);
        playerInfo_4_3->setObjectName("playerInfo_4_3");

        playerInfoLayout_4->addWidget(playerInfo_4_3);


        playerMainLayout_4->addLayout(playerInfoLayout_4);

        playerMainLayout_4->setStretch(0, 3);
        playerMainLayout_4->setStretch(1, 4);

        playerLayout_4->addLayout(playerMainLayout_4);

        playerLayout_4->setStretch(0, 1);
        playerLayout_4->setStretch(1, 5);

        mainLayout->addLayout(playerLayout_4, 2, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer, 1, 0, 1, 1);

        playerLayout_2 = new QVBoxLayout();
        playerLayout_2->setObjectName("playerLayout_2");
        playerNickname_2 = new QLabel(gameMainWidget);
        playerNickname_2->setObjectName("playerNickname_2");
        playerNickname_2->setFont(font);

        playerLayout_2->addWidget(playerNickname_2);

        playerMainLayout_2 = new QHBoxLayout();
        playerMainLayout_2->setObjectName("playerMainLayout_2");
        playerAvatarGraphics_2 = new QGraphicsView(gameMainWidget);
        playerAvatarGraphics_2->setObjectName("playerAvatarGraphics_2");

        playerMainLayout_2->addWidget(playerAvatarGraphics_2);

        playerInfoLayout_2 = new QVBoxLayout();
        playerInfoLayout_2->setObjectName("playerInfoLayout_2");
        playerInfo_2_1 = new QLabel(gameMainWidget);
        playerInfo_2_1->setObjectName("playerInfo_2_1");

        playerInfoLayout_2->addWidget(playerInfo_2_1);

        playerInfo_2_2 = new QLabel(gameMainWidget);
        playerInfo_2_2->setObjectName("playerInfo_2_2");

        playerInfoLayout_2->addWidget(playerInfo_2_2);

        playerInfo_2_3 = new QLabel(gameMainWidget);
        playerInfo_2_3->setObjectName("playerInfo_2_3");

        playerInfoLayout_2->addWidget(playerInfo_2_3);


        playerMainLayout_2->addLayout(playerInfoLayout_2);

        playerMainLayout_2->setStretch(0, 3);
        playerMainLayout_2->setStretch(1, 4);

        playerLayout_2->addLayout(playerMainLayout_2);

        playerLayout_2->setStretch(0, 1);
        playerLayout_2->setStretch(1, 5);

        mainLayout->addLayout(playerLayout_2, 0, 2, 1, 1);

        playerLayout_1 = new QVBoxLayout();
        playerLayout_1->setObjectName("playerLayout_1");
        playerNickname_1 = new QLabel(gameMainWidget);
        playerNickname_1->setObjectName("playerNickname_1");
        playerNickname_1->setFont(font);

        playerLayout_1->addWidget(playerNickname_1);

        playerMainLayout_1 = new QHBoxLayout();
        playerMainLayout_1->setObjectName("playerMainLayout_1");
        playerAvatarGraphics_1 = new QGraphicsView(gameMainWidget);
        playerAvatarGraphics_1->setObjectName("playerAvatarGraphics_1");

        playerMainLayout_1->addWidget(playerAvatarGraphics_1);

        playerInfoLayout_1 = new QVBoxLayout();
        playerInfoLayout_1->setObjectName("playerInfoLayout_1");
        playerInfo_1_1 = new QLabel(gameMainWidget);
        playerInfo_1_1->setObjectName("playerInfo_1_1");

        playerInfoLayout_1->addWidget(playerInfo_1_1);

        playerInfo_1_2 = new QLabel(gameMainWidget);
        playerInfo_1_2->setObjectName("playerInfo_1_2");

        playerInfoLayout_1->addWidget(playerInfo_1_2);

        playerInfo_1_3 = new QLabel(gameMainWidget);
        playerInfo_1_3->setObjectName("playerInfo_1_3");

        playerInfoLayout_1->addWidget(playerInfo_1_3);


        playerMainLayout_1->addLayout(playerInfoLayout_1);

        playerMainLayout_1->setStretch(0, 3);
        playerMainLayout_1->setStretch(1, 4);

        playerLayout_1->addLayout(playerMainLayout_1);

        playerLayout_1->setStretch(0, 1);
        playerLayout_1->setStretch(1, 5);

        mainLayout->addLayout(playerLayout_1, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        diceLabel_1 = new QLabel(gameMainWidget);
        diceLabel_1->setObjectName("diceLabel_1");
        diceLabel_1->setMaximumSize(QSize(50, 50));
        diceLabel_1->setPixmap(QPixmap(QString::fromUtf8(":/resources/dice1.gif")));
        diceLabel_1->setScaledContents(true);

        horizontalLayout->addWidget(diceLabel_1);

        diceLabel_2 = new QLabel(gameMainWidget);
        diceLabel_2->setObjectName("diceLabel_2");
        diceLabel_2->setMaximumSize(QSize(50, 50));
        diceLabel_2->setPixmap(QPixmap(QString::fromUtf8(":/resources/dice1.gif")));
        diceLabel_2->setScaledContents(true);

        horizontalLayout->addWidget(diceLabel_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        horizontalLayout->setStretch(0, 4);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(3, 4);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        rollDiceButton = new QPushButton(gameMainWidget);
        rollDiceButton->setObjectName("rollDiceButton");

        horizontalLayout_2->addWidget(rollDiceButton);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        verticalLayout->setStretch(0, 4);
        verticalLayout->setStretch(1, 2);
        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 15);

        mainLayout->addLayout(verticalLayout, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        mainLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        playerLayout_3 = new QVBoxLayout();
        playerLayout_3->setObjectName("playerLayout_3");
        playerNickname_3 = new QLabel(gameMainWidget);
        playerNickname_3->setObjectName("playerNickname_3");
        playerNickname_3->setFont(font);

        playerLayout_3->addWidget(playerNickname_3);

        playerMainLayout_3 = new QHBoxLayout();
        playerMainLayout_3->setObjectName("playerMainLayout_3");
        playerAvatarGraphics_3 = new QGraphicsView(gameMainWidget);
        playerAvatarGraphics_3->setObjectName("playerAvatarGraphics_3");

        playerMainLayout_3->addWidget(playerAvatarGraphics_3);

        playerInfoLayout_3 = new QVBoxLayout();
        playerInfoLayout_3->setObjectName("playerInfoLayout_3");
        playerInfo_3_1 = new QLabel(gameMainWidget);
        playerInfo_3_1->setObjectName("playerInfo_3_1");

        playerInfoLayout_3->addWidget(playerInfo_3_1);

        playerInfo_3_2 = new QLabel(gameMainWidget);
        playerInfo_3_2->setObjectName("playerInfo_3_2");

        playerInfoLayout_3->addWidget(playerInfo_3_2);

        playerInfo_3_3 = new QLabel(gameMainWidget);
        playerInfo_3_3->setObjectName("playerInfo_3_3");

        playerInfoLayout_3->addWidget(playerInfo_3_3);


        playerMainLayout_3->addLayout(playerInfoLayout_3);

        playerMainLayout_3->setStretch(0, 3);
        playerMainLayout_3->setStretch(1, 4);

        playerLayout_3->addLayout(playerMainLayout_3);

        playerLayout_3->setStretch(0, 1);
        playerLayout_3->setStretch(1, 5);

        mainLayout->addLayout(playerLayout_3, 2, 0, 1, 1);

        mainLayout->setRowStretch(0, 1);
        mainLayout->setRowStretch(1, 4);
        mainLayout->setRowStretch(2, 1);
        mainLayout->setColumnStretch(0, 1);
        mainLayout->setColumnStretch(1, 4);
        mainLayout->setColumnStretch(2, 1);

        retranslateUi(gameMainWidget);

        QMetaObject::connectSlotsByName(gameMainWidget);
    } // setupUi

    void retranslateUi(QWidget *gameMainWidget)
    {
        gameMainWidget->setWindowTitle(QCoreApplication::translate("gameMainWidget", "Monopoly Game", nullptr));
        playerNickname_4->setProperty("text_5389528734", QVariant(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr)));
        playerInfo_4_1->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_4_2->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_4_3->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerNickname_2->setProperty("text_10", QVariant(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr)));
        playerInfo_2_1->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_2_2->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_2_3->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerNickname_1->setProperty("textaf", QVariant(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr)));
        playerInfo_1_1->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_1_2->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_1_3->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        diceLabel_1->setText(QString());
        diceLabel_2->setText(QString());
        rollDiceButton->setText(QCoreApplication::translate("gameMainWidget", "Roll dice", nullptr));
        playerNickname_3->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_3_1->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_3_2->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
        playerInfo_3_3->setText(QCoreApplication::translate("gameMainWidget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gameMainWidget: public Ui_gameMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEMAINWIDGET_H
