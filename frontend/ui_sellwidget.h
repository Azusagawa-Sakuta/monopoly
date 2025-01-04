/********************************************************************************
** Form generated from reading UI file 'sellwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELLWIDGET_H
#define UI_SELLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sellWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelIntro;
    QGridLayout *gridLayout;
    QLabel *label2;
    QLabel *label1;
    QTreeView *treeView1;
    QTreeView *treeView2;
    QCommandLinkButton *sellButton;
    QCommandLinkButton *cancelButton;

    void setupUi(QWidget *sellWidget)
    {
        if (sellWidget->objectName().isEmpty())
            sellWidget->setObjectName("sellWidget");
        sellWidget->resize(640, 480);
        verticalLayout = new QVBoxLayout(sellWidget);
        verticalLayout->setObjectName("verticalLayout");
        labelIntro = new QLabel(sellWidget);
        labelIntro->setObjectName("labelIntro");

        verticalLayout->addWidget(labelIntro);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label2 = new QLabel(sellWidget);
        label2->setObjectName("label2");

        gridLayout->addWidget(label2, 0, 1, 1, 1);

        label1 = new QLabel(sellWidget);
        label1->setObjectName("label1");

        gridLayout->addWidget(label1, 0, 0, 1, 1);

        treeView1 = new QTreeView(sellWidget);
        treeView1->setObjectName("treeView1");

        gridLayout->addWidget(treeView1, 1, 0, 1, 1);

        treeView2 = new QTreeView(sellWidget);
        treeView2->setObjectName("treeView2");

        gridLayout->addWidget(treeView2, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        sellButton = new QCommandLinkButton(sellWidget);
        sellButton->setObjectName("sellButton");
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        sellButton->setFont(font);

        verticalLayout->addWidget(sellButton);

        cancelButton = new QCommandLinkButton(sellWidget);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setFont(font);

        verticalLayout->addWidget(cancelButton);


        retranslateUi(sellWidget);

        QMetaObject::connectSlotsByName(sellWidget);
    } // setupUi

    void retranslateUi(QWidget *sellWidget)
    {
        sellWidget->setWindowTitle(QString());
        labelIntro->setText(QCoreApplication::translate("sellWidget", "You have 11451$, but have to pay 19198$. Still need to pay $.", nullptr));
        label2->setText(QCoreApplication::translate("sellWidget", "Total Value: 1111$", nullptr));
        label1->setText(QCoreApplication::translate("sellWidget", "Your Plots", nullptr));
        sellButton->setText(QCoreApplication::translate("sellWidget", "Sacrifice", nullptr));
        cancelButton->setText(QCoreApplication::translate("sellWidget", "All end", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sellWidget: public Ui_sellWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELLWIDGET_H
