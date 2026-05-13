QT += core gui widgets

TEMPLATE = app
CONFIG += c++17

SOURCES += \
    main.cpp \
    ui/LinkedListWidget.cpp \
    ui/MainWindow.cpp \
    logic/LinkedList.cpp

HEADERS += \
    logic/LinkedList.h \
    ui/LinkedListWidget.h \
    ui/MainWindow.h

FORMS += \
    ui/MainWindow.ui

INCLUDEPATH += logic ui
