QT += core gui widgets

TEMPLATE = app
CONFIG += c++17

SOURCES += \
    logic/AVL.cpp \
    logic/BST.cpp \
    logic/DoubleLinkedList.cpp \
    logic/Persistence.cpp \
    logic/Queue.cpp \
    logic/Stack.cpp \
    main.cpp \
    ui/AVLWidget.cpp \
    ui/BSTWidget.cpp \
    ui/DLLWidget.cpp \
    ui/LinkedListWidget.cpp \
    ui/MainWindow.cpp \
    logic/LinkedList.cpp \
    ui/QueueWidget.cpp \
    ui/StackWidget.cpp

HEADERS += \
    logic/AVL.h \
    logic/BST.h \
    logic/DoubleLinkedList.h \
    logic/LinkedList.h \
    logic/Persistence.h \
    logic/Queue.h \
    logic/Stack.h \
    ui/AVLWidget.h \
    ui/BSTWidget.h \
    ui/DLLWidget.h \
    ui/LinkedListWidget.h \
    ui/MainWindow.h \
    ui/QueueWidget.h \
    ui/StackWidget.h

FORMS += \
    ui/MainWindow.ui

INCLUDEPATH += logic ui
