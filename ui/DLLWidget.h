#pragma once
#include <QWidget>
#include "../logic/DoubleLinkedList.h"

// WIDGET DE VISUALIZACIÓN DE LA DOUBLE LINKED LIST
class DLLWidget : public QWidget {
    Q_OBJECT
public:
    explicit DLLWidget(QWidget* parent = nullptr);
    void setList(DoubleLinkedList* list);
    void highlightNode(int val);
    void clearHighlight();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    DoubleLinkedList* dll;
    int highlightedVal;
    bool highlightActive;

    static const int NODE_W = 60;
    static const int NODE_H = 40;
    static const int GAP = 36;
    static const int START_X = 80;
    static const int START_Y = 60;
};
