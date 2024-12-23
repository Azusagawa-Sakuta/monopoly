/********************************************************************************
** Form generated from reading UI file 'gamemainwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEMAINWIDGET_H
#define UI_GAMEMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gameMainWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *gameMainWidget)
    {
        if (gameMainWidget->objectName().isEmpty())
            gameMainWidget->setObjectName("gameMainWidget");
        gameMainWidget->resize(1920, 860);
        verticalLayout = new QVBoxLayout(gameMainWidget);
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName("verticalLayout");
        graphicsView = new QGraphicsView(gameMainWidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setMinimumSize(QSize(1000, 600));
        graphicsView->setMaximumSize(QSize(1000, 600));

        verticalLayout->addWidget(graphicsView, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);


        retranslateUi(gameMainWidget);

        QMetaObject::connectSlotsByName(gameMainWidget);
    } // setupUi

    void retranslateUi(QWidget *gameMainWidget)
    {
        gameMainWidget->setWindowTitle(QCoreApplication::translate("gameMainWidget", "Monopoly Game", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gameMainWidget: public Ui_gameMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEMAINWIDGET_H
