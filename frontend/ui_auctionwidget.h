/********************************************************************************
** Form generated from reading UI file 'auctionwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUCTIONWIDGET_H
#define UI_AUCTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_auctionWidget
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *auctionLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *tileGraphics;
    QLabel *tileLabel;
    QSpacerItem *verticalSpacer_4;
    QGridLayout *gridLayout;
    QGraphicsView *playerAvatar_3;
    QLabel *playerLabel_4;
    QLabel *playerLabel_1;
    QGraphicsView *playerAvatar_2;
    QGraphicsView *playerAvatar_1;
    QLabel *playerLabel_2;
    QLabel *playerLabel_3;
    QGraphicsView *playerAvatar_4;
    QLabel *playerValue_1;
    QLabel *playerValue_2;
    QLabel *playerValue_3;
    QLabel *playerValue_4;
    QHBoxLayout *horizontalLayout;
    QLabel *currentBidLabel;
    QSpinBox *bidInput;
    QPushButton *bidButton;
    QPushButton *passButton;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *auctionWidget)
    {
        if (auctionWidget->objectName().isEmpty())
            auctionWidget->setObjectName("auctionWidget");
        auctionWidget->setWindowModality(Qt::WindowModality::NonModal);
        auctionWidget->resize(640, 480);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(auctionWidget->sizePolicy().hasHeightForWidth());
        auctionWidget->setSizePolicy(sizePolicy);
        auctionWidget->setMinimumSize(QSize(640, 480));
        auctionWidget->setMaximumSize(QSize(640, 480));
        auctionWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        auctionWidget->setWindowOpacity(0.900000000000000);
        verticalLayout = new QVBoxLayout(auctionWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        auctionLabel = new QLabel(auctionWidget);
        auctionLabel->setObjectName("auctionLabel");
        QFont font;
        font.setFamilies({QString::fromUtf8("Script MT")});
        font.setPointSize(20);
        auctionLabel->setFont(font);
        auctionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(auctionLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        tileGraphics = new QGraphicsView(auctionWidget);
        tileGraphics->setObjectName("tileGraphics");
        sizePolicy.setHeightForWidth(tileGraphics->sizePolicy().hasHeightForWidth());
        tileGraphics->setSizePolicy(sizePolicy);
        tileGraphics->setMinimumSize(QSize(128, 128));
        tileGraphics->setMaximumSize(QSize(128, 128));
        tileGraphics->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        tileGraphics->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        horizontalLayout_2->addWidget(tileGraphics);

        tileLabel = new QLabel(auctionWidget);
        tileLabel->setObjectName("tileLabel");

        horizontalLayout_2->addWidget(tileLabel);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        playerAvatar_3 = new QGraphicsView(auctionWidget);
        playerAvatar_3->setObjectName("playerAvatar_3");
        sizePolicy.setHeightForWidth(playerAvatar_3->sizePolicy().hasHeightForWidth());
        playerAvatar_3->setSizePolicy(sizePolicy);
        playerAvatar_3->setMinimumSize(QSize(128, 128));
        playerAvatar_3->setMaximumSize(QSize(128, 128));

        gridLayout->addWidget(playerAvatar_3, 0, 2, 1, 1);

        playerLabel_4 = new QLabel(auctionWidget);
        playerLabel_4->setObjectName("playerLabel_4");

        gridLayout->addWidget(playerLabel_4, 1, 3, 1, 1);

        playerLabel_1 = new QLabel(auctionWidget);
        playerLabel_1->setObjectName("playerLabel_1");

        gridLayout->addWidget(playerLabel_1, 1, 0, 1, 1);

        playerAvatar_2 = new QGraphicsView(auctionWidget);
        playerAvatar_2->setObjectName("playerAvatar_2");
        sizePolicy.setHeightForWidth(playerAvatar_2->sizePolicy().hasHeightForWidth());
        playerAvatar_2->setSizePolicy(sizePolicy);
        playerAvatar_2->setMinimumSize(QSize(128, 128));
        playerAvatar_2->setMaximumSize(QSize(128, 128));

        gridLayout->addWidget(playerAvatar_2, 0, 1, 1, 1);

        playerAvatar_1 = new QGraphicsView(auctionWidget);
        playerAvatar_1->setObjectName("playerAvatar_1");
        sizePolicy.setHeightForWidth(playerAvatar_1->sizePolicy().hasHeightForWidth());
        playerAvatar_1->setSizePolicy(sizePolicy);
        playerAvatar_1->setMinimumSize(QSize(128, 128));
        playerAvatar_1->setMaximumSize(QSize(128, 128));

        gridLayout->addWidget(playerAvatar_1, 0, 0, 1, 1);

        playerLabel_2 = new QLabel(auctionWidget);
        playerLabel_2->setObjectName("playerLabel_2");

        gridLayout->addWidget(playerLabel_2, 1, 1, 1, 1);

        playerLabel_3 = new QLabel(auctionWidget);
        playerLabel_3->setObjectName("playerLabel_3");

        gridLayout->addWidget(playerLabel_3, 1, 2, 1, 1);

        playerAvatar_4 = new QGraphicsView(auctionWidget);
        playerAvatar_4->setObjectName("playerAvatar_4");
        sizePolicy.setHeightForWidth(playerAvatar_4->sizePolicy().hasHeightForWidth());
        playerAvatar_4->setSizePolicy(sizePolicy);
        playerAvatar_4->setMinimumSize(QSize(128, 128));
        playerAvatar_4->setMaximumSize(QSize(128, 128));

        gridLayout->addWidget(playerAvatar_4, 0, 3, 1, 1);

        playerValue_1 = new QLabel(auctionWidget);
        playerValue_1->setObjectName("playerValue_1");

        gridLayout->addWidget(playerValue_1, 2, 0, 1, 1);

        playerValue_2 = new QLabel(auctionWidget);
        playerValue_2->setObjectName("playerValue_2");

        gridLayout->addWidget(playerValue_2, 2, 1, 1, 1);

        playerValue_3 = new QLabel(auctionWidget);
        playerValue_3->setObjectName("playerValue_3");

        gridLayout->addWidget(playerValue_3, 2, 2, 1, 1);

        playerValue_4 = new QLabel(auctionWidget);
        playerValue_4->setObjectName("playerValue_4");

        gridLayout->addWidget(playerValue_4, 2, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        currentBidLabel = new QLabel(auctionWidget);
        currentBidLabel->setObjectName("currentBidLabel");
        currentBidLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(currentBidLabel);

        bidInput = new QSpinBox(auctionWidget);
        bidInput->setObjectName("bidInput");

        horizontalLayout->addWidget(bidInput);

        bidButton = new QPushButton(auctionWidget);
        bidButton->setObjectName("bidButton");
        sizePolicy.setHeightForWidth(bidButton->sizePolicy().hasHeightForWidth());
        bidButton->setSizePolicy(sizePolicy);
        bidButton->setMaximumSize(QSize(64, 16777215));
        bidButton->setFlat(false);

        horizontalLayout->addWidget(bidButton);

        passButton = new QPushButton(auctionWidget);
        passButton->setObjectName("passButton");
        sizePolicy.setHeightForWidth(passButton->sizePolicy().hasHeightForWidth());
        passButton->setSizePolicy(sizePolicy);
        passButton->setMaximumSize(QSize(64, 16777215));

        horizontalLayout->addWidget(passButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        retranslateUi(auctionWidget);

        QMetaObject::connectSlotsByName(auctionWidget);
    } // setupUi

    void retranslateUi(QWidget *auctionWidget)
    {
        auctionWidget->setWindowTitle(QCoreApplication::translate("auctionWidget", "Auction", nullptr));
        auctionLabel->setText(QCoreApplication::translate("auctionWidget", "Auction", nullptr));
        tileLabel->setText(QCoreApplication::translate("auctionWidget", "TextLabel", nullptr));
        playerLabel_4->setText(QString());
        playerLabel_1->setText(QCoreApplication::translate("auctionWidget", "TextLabel", nullptr));
        playerLabel_2->setText(QCoreApplication::translate("auctionWidget", "TextLabel", nullptr));
        playerLabel_3->setText(QString());
        playerValue_1->setText(QCoreApplication::translate("auctionWidget", "TextLabel", nullptr));
        playerValue_2->setText(QCoreApplication::translate("auctionWidget", "TextLabel", nullptr));
        playerValue_3->setText(QString());
        playerValue_4->setText(QString());
        currentBidLabel->setText(QCoreApplication::translate("auctionWidget", "New bid", nullptr));
        bidButton->setText(QCoreApplication::translate("auctionWidget", "Bid!", nullptr));
        passButton->setText(QCoreApplication::translate("auctionWidget", "Pass...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class auctionWidget: public Ui_auctionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUCTIONWIDGET_H
