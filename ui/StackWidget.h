#pragma once
#include <QWidget>
#include "../logic/Stack.h"

// WIDGET DE VISUALIZACIÓN DEL STACK
class StackWidget : public QWidget {
    Q_OBJECT
public:
    explicit StackWidget(QWidget* parent = nullptr);
    void setStack(Stack* stack);
    void highlightNode(int val);
    void clearHighlight();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Stack* st;
    int highlightedVal;
    bool highlightActive;

    static const int NODE_W = 80;
    static const int NODE_H = 40;
    static const int GAP = 28;
    static const int START_X = 20;
    static const int START_Y = 50;
};
