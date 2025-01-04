/********************************************************************************
** Form generated from reading UI file 'startwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_startWidget
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *mainLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *gameTitleLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *startButton;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *manualButton;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *closeButton;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *aboutUsButton;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *startWidget)
    {
        if (startWidget->objectName().isEmpty())
            startWidget->setObjectName("startWidget");
        startWidget->resize(1920, 1080);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(startWidget->sizePolicy().hasHeightForWidth());
        startWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(startWidget);
        verticalLayout->setObjectName("verticalLayout");
        mainLayout = new QVBoxLayout();
        mainLayout->setObjectName("mainLayout");
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer_2);

        gameTitleLabel = new QLabel(startWidget);
        gameTitleLabel->setObjectName("gameTitleLabel");
        QFont font;
        font.setFamilies({QString::fromUtf8("Vivaldi")});
        font.setPointSize(14);
        font.setItalic(true);
        gameTitleLabel->setFont(font);
        gameTitleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainLayout->addWidget(gameTitleLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        startButton = new QPushButton(startWidget);
        startButton->setObjectName("startButton");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Comic Sans MS")});
        startButton->setFont(font1);

        horizontalLayout_5->addWidget(startButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        mainLayout->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        manualButton = new QPushButton(startWidget);
        manualButton->setObjectName("manualButton");
        manualButton->setFont(font1);

        horizontalLayout->addWidget(manualButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        mainLayout->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        closeButton = new QPushButton(startWidget);
        closeButton->setObjectName("closeButton");
        closeButton->setFont(font1);

        horizontalLayout_4->addWidget(closeButton);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        mainLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        aboutUsButton = new QPushButton(startWidget);
        aboutUsButton->setObjectName("aboutUsButton");
        aboutUsButton->setFont(font1);

        horizontalLayout_3->addWidget(aboutUsButton);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);


        mainLayout->addLayout(horizontalLayout_3);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer_3);

        mainLayout->setStretch(0, 5);
        mainLayout->setStretch(1, 12);
        mainLayout->setStretch(2, 1);
        mainLayout->setStretch(3, 2);
        mainLayout->setStretch(4, 2);
        mainLayout->setStretch(5, 2);
        mainLayout->setStretch(6, 2);
        mainLayout->setStretch(7, 6);

        verticalLayout->addLayout(mainLayout);


        retranslateUi(startWidget);

        QMetaObject::connectSlotsByName(startWidget);
    } // setupUi

    void retranslateUi(QWidget *startWidget)
    {
        startWidget->setWindowTitle(QCoreApplication::translate("startWidget", "Start Page", nullptr));
        gameTitleLabel->setText(QCoreApplication::translate("startWidget", "<html><head/><body><p align=\"center\"><span style=\" font-size:36pt; font-weight:700;\">Monopoly</span></p></body></html>", nullptr));
        startButton->setText(QCoreApplication::translate("startWidget", "Start!", nullptr));
        manualButton->setText(QCoreApplication::translate("startWidget", "Manual", nullptr));
        closeButton->setText(QCoreApplication::translate("startWidget", "Close", nullptr));
        aboutUsButton->setText(QCoreApplication::translate("startWidget", "About us", nullptr));
    } // retranslateUi

};

namespace Ui {
    class startWidget: public Ui_startWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTWIDGET_H
