#pragma once
#include <QWidget>
#include <QTimer>
#include "../logic/LinkedList.h"

// WIDGET DE VISUALIZACIÓN DE LA LISTA
// QWidget personalizado que dibuja la lista enlazada directamente sobre el canvas con QPainter
class LinkedListWidget : public QWidget {
    Q_OBJECT
public:
    explicit LinkedListWidget(QWidget* parent = nullptr);
    void setList(LinkedList* list);
    void highlightNode(int val);
    void clearHighlight();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onAnimationTick();

private:
    LinkedList* ll;
    int highlightedVal;
    bool highlightActive;
    float animOffset;
    QTimer* animTimer;

    static const int NODE_W = 60;
    static const int NODE_H = 40;
    static const int GAP = 30;
    static const int START_X = 20;
    static const int START_Y = 50;
};
