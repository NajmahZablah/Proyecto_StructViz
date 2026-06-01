#include "QueueWidget.h"
#include <QPainter>
#include <QFont>

// CONSTRUCTOR
QueueWidget::QueueWidget(QWidget* parent)
    : QWidget(parent), q(nullptr), highlightedVal(-1), highlightActive(false)
{
    setMinimumHeight(120);
}

// SETQUEUE
void QueueWidget::setQueue(Queue* queue) {
    q = queue;
    update();
}

// HIGHLIGHT
void QueueWidget::highlightNode(int val) {
    highlightedVal = val;
    highlightActive = true;
    update();
}

void QueueWidget::clearHighlight() {
    highlightActive = false;
    update();
}

// PAINTEVENT
void QueueWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Fondo
    p.fillRect(rect(), QColor("#f0fff8"));

    if (!q || q->isEmpty()) {
        p.setPen(QColor("#70a890"));
        p.setFont(QFont("Segoe UI", 11));
        p.drawText(rect(), Qt::AlignCenter, "Queue vacía  →  nullptr");
        return;
    }

    Node* curr = q->getFront();
    Node* back = q->getBack();
    int x = START_X;

    while (curr) {
        bool isFront = (curr == q->getFront());
        bool isBack = (curr == back);
        bool isHighlighted = (highlightActive && curr->value == highlightedVal);

        // Colores - paleta monocromática verde
        QColor nodeColor = isHighlighted ? QColor("#f0a0b8")
                           : isFront ? QColor("#3aaa70")
                           : isBack ? QColor("#c0ecd8")
                                    : QColor("#78c8a0");
        QColor borderColor = isHighlighted ? QColor("#d06080")
                             : isFront ? QColor("#207848")
                             : isBack ? QColor("#60a880")
                                      : QColor("#3a8858");
        QColor textColor = isHighlighted ? QColor("#803050")
                           : isFront ? QColor("#ffffff")
                           : isBack ? QColor("#205040")
                                    : QColor("#ffffff");

        // Etiqueta "front" o "back"
        if (isFront || isBack) {
            QColor labelColor = isFront ? QColor("#307060") : QColor("#508030");
            p.setPen(labelColor);
            p.setFont(QFont("Segoe UI", 8, QFont::Bold));
            p.drawText(QRect(x, START_Y - 20, NODE_W, 16),
                       Qt::AlignCenter,
                       isFront ? "front" : "back");
        }

        // Nodo
        p.setBrush(nodeColor);
        p.setPen(QPen(borderColor, 2));
        p.drawRoundedRect(x, START_Y, NODE_W, NODE_H, 10, 10);

        // Valor
        p.setPen(textColor);
        p.setFont(QFont("Segoe UI", 13, QFont::Bold));
        p.drawText(QRect(x, START_Y, NODE_W, NODE_H),
                   Qt::AlignCenter,
                   QString::number(curr->value));

        // Flecha o badge nullptr
        if (curr->next) {
            int arrowY = START_Y + NODE_H / 2;
            int arrowStartX = x + NODE_W + 4;
            int arrowEndX = x + NODE_W + GAP - 4;

            QColor arrowColor("#40a870");
            p.setPen(QPen(arrowColor, 4, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(arrowStartX, arrowY, arrowEndX - 11, arrowY);

            QPolygon arrow;
            arrow << QPoint(arrowEndX, arrowY)
                  << QPoint(arrowEndX - 11, arrowY - 6)
                  << QPoint(arrowEndX - 11, arrowY + 6);
            p.setBrush(arrowColor);
            p.setPen(Qt::NoPen);
            p.drawPolygon(arrow);

        } else {
            int arrowY = START_Y + NODE_H / 2;
            int badgeX = x + NODE_W + 8;
            int badgeY = arrowY - 10;
            int badgeW = 52;
            int badgeH = 20;

            QColor arrowColor("#40a870");
            p.setPen(QPen(arrowColor, 4, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(x + NODE_W + 4, arrowY, badgeX, arrowY);

            // Badge
            p.setBrush(QColor("#d0f0e0"));
            p.setPen(QPen(QColor("#60b890"), 1.5));
            p.drawRoundedRect(badgeX, badgeY, badgeW, badgeH, 6, 6);

            p.setPen(QColor("#306050"));
            p.setFont(QFont("Segoe UI", 8, QFont::StyleItalic));
            p.drawText(QRect(badgeX, badgeY, badgeW, badgeH),
                       Qt::AlignCenter, "nullptr");
        }

        x += NODE_W + GAP;
        curr = curr->next;
    }
}
