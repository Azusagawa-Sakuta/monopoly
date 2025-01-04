QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TARGET = monopoly

SOURCES += \
    auctionwidget.cpp \
    gamemainwidget.cpp \
    main.cpp \
    ../backend/constant.cpp \
    ../backend/game.cpp \
    ../backend/utils.cpp \
    sellwidget.cpp \
    startwidget.cpp \
    widget.cpp

HEADERS += \
    ../backend/constant.h \
    ../backend/game.h \
    ../backend/utils.h \
    auctionwidget.h \
    gamemainwidget.h \
    sellwidget.h \
    startwidget.h \
    widget.h \
    ui_auctionwidget.h \
    ui_gamemainwidget.h \
    ui_sellwidget.h \
    ui_startwidget.h \
    ui_widget.h

RESOURCES += resources/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
