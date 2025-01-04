/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

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

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *mainLayout;
    QHBoxLayout *titleLayout;
    QPushButton *manualButton;
    QSpacerItem *horizontalSpacer;
    QLabel *gameTitle;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *profileLayout;
    QVBoxLayout *playerLayout_1;
    QGraphicsView *playerProfilePhoto_1;
    QSpacerItem *verticalSpacer_1;
    QPushButton *addComputer_1;
    QPushButton *addPlayer_1;
    QVBoxLayout *playerLayout_2;
    QGraphicsView *playerProfilePhoto_2;
    QSpacerItem *verticalSpacer_9;
    QPushButton *addComputer_2;
    QPushButton *addPlayer_2;
    QVBoxLayout *playerLayout_3;
    QGraphicsView *playerProfilePhoto_3;
    QSpacerItem *verticalSpacer_3;
    QPushButton *addComputer_3;
    QPushButton *addPlayer_3;
    QVBoxLayout *playerLayout_4;
    QGraphicsView *playerProfilePhoto_4;
    QSpacerItem *verticalSpacer_4;
    QPushButton *addComputer_4;
    QPushButton *addPlayer_4;
    QHBoxLayout *photoListLayout;
    QGraphicsView *profilePhotoList;
    QHBoxLayout *functionLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *commitButton;
    QPushButton *cancelButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(1800, 860);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamilies({QString::fromUtf8("Script MT")});
        font.setPointSize(14);
        Widget->setFont(font);
        Widget->setWindowOpacity(1.000000000000000);
        gridLayout = new QGridLayout(Widget);
        gridLayout->setObjectName("gridLayout");
        mainLayout = new QVBoxLayout();
        mainLayout->setObjectName("mainLayout");
        mainLayout->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
        titleLayout = new QHBoxLayout();
        titleLayout->setObjectName("titleLayout");
        manualButton = new QPushButton(Widget);
        manualButton->setObjectName("manualButton");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Script MT")});
        font1.setPointSize(16);
        manualButton->setFont(font1);

        titleLayout->addWidget(manualButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        titleLayout->addItem(horizontalSpacer);

        gameTitle = new QLabel(Widget);
        gameTitle->setObjectName("gameTitle");
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Bell MT")});
        font2.setPointSize(24);
        gameTitle->setFont(font2);

        titleLayout->addWidget(gameTitle);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        titleLayout->addItem(horizontalSpacer_2);

        titleLayout->setStretch(0, 1);
        titleLayout->setStretch(1, 12);
        titleLayout->setStretch(2, 8);
        titleLayout->setStretch(3, 13);

        mainLayout->addLayout(titleLayout);

        profileLayout = new QHBoxLayout();
        profileLayout->setObjectName("profileLayout");
        playerLayout_1 = new QVBoxLayout();
        playerLayout_1->setObjectName("playerLayout_1");
        playerProfilePhoto_1 = new QGraphicsView(Widget);
        playerProfilePhoto_1->setObjectName("playerProfilePhoto_1");

        playerLayout_1->addWidget(playerProfilePhoto_1);

        verticalSpacer_1 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        playerLayout_1->addItem(verticalSpacer_1);

        addComputer_1 = new QPushButton(Widget);
        addComputer_1->setObjectName("addComputer_1");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Source Code Pro")});
        font3.setPointSize(14);
        addComputer_1->setFont(font3);

        playerLayout_1->addWidget(addComputer_1);

        addPlayer_1 = new QPushButton(Widget);
        addPlayer_1->setObjectName("addPlayer_1");
        addPlayer_1->setFont(font3);

        playerLayout_1->addWidget(addPlayer_1);

        playerLayout_1->setStretch(0, 13);
        playerLayout_1->setStretch(1, 1);
        playerLayout_1->setStretch(2, 1);
        playerLayout_1->setStretch(3, 1);

        profileLayout->addLayout(playerLayout_1);

        playerLayout_2 = new QVBoxLayout();
        playerLayout_2->setObjectName("playerLayout_2");
        playerProfilePhoto_2 = new QGraphicsView(Widget);
        playerProfilePhoto_2->setObjectName("playerProfilePhoto_2");

        playerLayout_2->addWidget(playerProfilePhoto_2);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        playerLayout_2->addItem(verticalSpacer_9);

        addComputer_2 = new QPushButton(Widget);
        addComputer_2->setObjectName("addComputer_2");
        addComputer_2->setFont(font3);

        playerLayout_2->addWidget(addComputer_2);

        addPlayer_2 = new QPushButton(Widget);
        addPlayer_2->setObjectName("addPlayer_2");
        addPlayer_2->setFont(font3);

        playerLayout_2->addWidget(addPlayer_2);

        playerLayout_2->setStretch(0, 13);
        playerLayout_2->setStretch(1, 1);
        playerLayout_2->setStretch(2, 1);
        playerLayout_2->setStretch(3, 1);

        profileLayout->addLayout(playerLayout_2);

        playerLayout_3 = new QVBoxLayout();
        playerLayout_3->setObjectName("playerLayout_3");
        playerProfilePhoto_3 = new QGraphicsView(Widget);
        playerProfilePhoto_3->setObjectName("playerProfilePhoto_3");

        playerLayout_3->addWidget(playerProfilePhoto_3);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        playerLayout_3->addItem(verticalSpacer_3);

        addComputer_3 = new QPushButton(Widget);
        addComputer_3->setObjectName("addComputer_3");
        addComputer_3->setFont(font3);

        playerLayout_3->addWidget(addComputer_3);

        addPlayer_3 = new QPushButton(Widget);
        addPlayer_3->setObjectName("addPlayer_3");
        addPlayer_3->setFont(font3);

        playerLayout_3->addWidget(addPlayer_3);

        playerLayout_3->setStretch(0, 13);
        playerLayout_3->setStretch(1, 1);
        playerLayout_3->setStretch(2, 1);
        playerLayout_3->setStretch(3, 1);

        profileLayout->addLayout(playerLayout_3);

        playerLayout_4 = new QVBoxLayout();
        playerLayout_4->setObjectName("playerLayout_4");
        playerProfilePhoto_4 = new QGraphicsView(Widget);
        playerProfilePhoto_4->setObjectName("playerProfilePhoto_4");

        playerLayout_4->addWidget(playerProfilePhoto_4);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        playerLayout_4->addItem(verticalSpacer_4);

        addComputer_4 = new QPushButton(Widget);
        addComputer_4->setObjectName("addComputer_4");
        addComputer_4->setFont(font3);

        playerLayout_4->addWidget(addComputer_4);

        addPlayer_4 = new QPushButton(Widget);
        addPlayer_4->setObjectName("addPlayer_4");
        addPlayer_4->setFont(font3);

        playerLayout_4->addWidget(addPlayer_4);

        playerLayout_4->setStretch(0, 13);
        playerLayout_4->setStretch(1, 1);
        playerLayout_4->setStretch(2, 1);
        playerLayout_4->setStretch(3, 1);

        profileLayout->addLayout(playerLayout_4);

        profileLayout->setStretch(0, 1);
        profileLayout->setStretch(1, 1);
        profileLayout->setStretch(2, 1);
        profileLayout->setStretch(3, 1);

        mainLayout->addLayout(profileLayout);

        photoListLayout = new QHBoxLayout();
        photoListLayout->setObjectName("photoListLayout");
        profilePhotoList = new QGraphicsView(Widget);
        profilePhotoList->setObjectName("profilePhotoList");

        photoListLayout->addWidget(profilePhotoList);


        mainLayout->addLayout(photoListLayout);

        functionLayout = new QHBoxLayout();
        functionLayout->setObjectName("functionLayout");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        functionLayout->addItem(horizontalSpacer_3);

        commitButton = new QPushButton(Widget);
        commitButton->setObjectName("commitButton");

        functionLayout->addWidget(commitButton);

        cancelButton = new QPushButton(Widget);
        cancelButton->setObjectName("cancelButton");

        functionLayout->addWidget(cancelButton);


        mainLayout->addLayout(functionLayout);

        mainLayout->setStretch(0, 1);
        mainLayout->setStretch(1, 15);
        mainLayout->setStretch(2, 6);
        mainLayout->setStretch(3, 1);

        gridLayout->addLayout(mainLayout, 0, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Select Player", nullptr));
        manualButton->setText(QCoreApplication::translate("Widget", "Hint", nullptr));
        gameTitle->setText(QCoreApplication::translate("Widget", "Please select the Role and Profile Photo", nullptr));
        addComputer_1->setText(QCoreApplication::translate("Widget", "Add Computer", nullptr));
        addPlayer_1->setText(QCoreApplication::translate("Widget", "Add Player", nullptr));
        addComputer_2->setText(QCoreApplication::translate("Widget", "Add Computer", nullptr));
        addPlayer_2->setText(QCoreApplication::translate("Widget", "Add Player", nullptr));
        addComputer_3->setText(QCoreApplication::translate("Widget", "Add Computer", nullptr));
        addPlayer_3->setText(QCoreApplication::translate("Widget", "Add Player", nullptr));
        addComputer_4->setText(QCoreApplication::translate("Widget", "Add Computer", nullptr));
        addPlayer_4->setText(QCoreApplication::translate("Widget", "Add Player", nullptr));
        commitButton->setText(QCoreApplication::translate("Widget", "Start!", nullptr));
        cancelButton->setText(QCoreApplication::translate("Widget", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
