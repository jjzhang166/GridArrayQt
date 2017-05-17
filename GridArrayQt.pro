
TEMPLATE = app

TARGET = GridArrayQt

QT += core widgets gui

HEADERS +=\
    ./GridArrayQt.h \
    ./QDrawArea.h \
    ./common.h \
    ./qtextctrl.h

SOURCES +=\
    ./QDrawArea.cpp \
    ./GridArrayQt.cpp \
    ./main.cpp \
    ./qtextctrl.cpp

FORMS += \
    GridArrayQt.ui

RESOURCES += GridArrayQt.qrc

TRANSLATIONS = lang_zh_cn.ts

