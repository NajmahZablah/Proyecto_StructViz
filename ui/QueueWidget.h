#pragma once
#include <QWidget>
#include "../logic/Queue.h"

// WIDGET DE VISUALIZACIÓN DE LA QUEUE
class QueueWidget : public QWidget {
    Q_OBJECT
public:
    explicit QueueWidget(QWidget* parent = nullptr);
    void setQueue(Queue* queue);
    void highlightNode(int val);
    void clearHighlight();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Queue* q;
    int highlightedVal;
    bool highlightActive;

    static const int NODE_W = 60;
    static const int NODE_H = 40;
    static const int GAP = 30;
    static const int START_X = 20;
    static const int START_Y = 50;
};
