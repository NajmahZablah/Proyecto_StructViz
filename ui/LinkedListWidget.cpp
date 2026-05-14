#include "LinkedListWidget.h"
#include <QPainter>
#include <QFont>

// CONSTRUCTOR
// Inicializa el estado interno y conecta el timer a su slot
LinkedListWidget::LinkedListWidget(QWidget* parent)
    : QWidget(parent), ll(nullptr), highlightedVal(-1),
    highlightActive(false), animOffset(1.0f)
{
    setMinimumHeight(120);
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout,
            this, &LinkedListWidget::onAnimationTick);
}

// SETLIST
// Actualiza la referencia y reinicia la animación
void LinkedListWidget::setList(LinkedList* list) {
    ll = list;
    animOffset = 0.0f;
    animTimer->start(16);
    update();
}

// HIGHLIGHT
// Resalta / Quita resaltado de un nodo
void LinkedListWidget::highlightNode(int val) {
    highlightedVal = val;
    highlightActive = true;
    update();
}

void LinkedListWidget::clearHighlight() {
    highlightActive = false;
    update();
}

// ANIMACIÓN
// Avanza animOffset hasta 1.0 y detiene el timer
void LinkedListWidget::onAnimationTick() {
    if (animOffset < 1.0f) {
        animOffset += 0.06f;
        if (animOffset >= 1.0f) {
            animOffset = 1.0f;
            animTimer->stop();
        }
        update();
    }
}

// PAINTEVENT
// Dibuja toda la lista nodo a nodo
void LinkedListWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Fondo
    p.fillRect(rect(), QColor("#f5f0ff"));

    if (!ll || !ll->getHead()) {
        p.setPen(QColor("#a090c0"));
        p.setFont(QFont("Segoe UI", 11));
        p.drawText(rect(), Qt::AlignCenter, "Lista vacía  →  nullptr");
        return;
    }

    Node* curr = ll->getHead();
    int x = START_X;
    int i = 0;

    while (curr) {
        int drawX = x - (int)((1.0f - animOffset) * 30);
        float alpha = animOffset;

        bool isHead = (i == 0);
        bool isTail = (curr->next == nullptr);
        bool isHighlighted = (highlightActive && curr->data == highlightedVal);

        // Colores pasteles
        QColor nodeColor = isHighlighted ? QColor("#f0a0b8")
                           : isHead        ? QColor("#c8a8f0")
                           : isTail        ? QColor("#a8d8c8")
                                         : QColor("#f0c0d8");
        QColor borderColor = isHighlighted ? QColor("#d06080")
                             : isHead        ? QColor("#9878d0")
                             : isTail        ? QColor("#78b8a8")
                                           : QColor("#d098b8");
        QColor textColor = isHighlighted ? QColor("#803050")
                           : isHead        ? QColor("#503090")
                           : isTail        ? QColor("#306050")
                                         : QColor("#804060");
        nodeColor.setAlphaF(alpha);
        borderColor.setAlphaF(alpha);
        textColor.setAlphaF(alpha);

        // Etiqueta head/tail
        if (isHead || isTail) {
            QColor labelColor = isHead ? QColor("#7050b0") : QColor("#307060");
            labelColor.setAlphaF(alpha);
            p.setPen(labelColor);
            p.setFont(QFont("Segoe UI", 8, QFont::Bold));
            p.drawText(QRect(drawX, START_Y - 20, NODE_W, 16),
                       Qt::AlignCenter,
                       isHead ? "head" : "tail");
        }

        // Nodo
        p.setBrush(nodeColor);
        p.setPen(QPen(borderColor, 2));
        p.drawRoundedRect(drawX, START_Y, NODE_W, NODE_H, 10, 10);

        // Valor
        p.setPen(textColor);
        p.setFont(QFont("Segoe UI", 13, QFont::Bold));
        p.drawText(QRect(drawX, START_Y, NODE_W, NODE_H),
                   Qt::AlignCenter,
                   QString::number(curr->data));

        // Flecha o nullptr
        if (curr->next) {
            int arrowStartX = drawX + NODE_W + 4;
            int arrowEndX   = drawX + NODE_W + GAP - 4;
            int arrowY      = START_Y + NODE_H / 2;

            QColor arrowColor("#b090c8");
            arrowColor.setAlphaF(alpha);
            p.setPen(QPen(arrowColor, 2));
            p.drawLine(arrowStartX, arrowY, arrowEndX - 6, arrowY);

            // Cabeza de flecha
            QPolygon arrow;
            arrow << QPoint(arrowEndX, arrowY)
                  << QPoint(arrowEndX - 8, arrowY - 5)
                  << QPoint(arrowEndX - 8, arrowY + 5);
            p.setBrush(arrowColor);
            p.setPen(Qt::NoPen);
            p.drawPolygon(arrow);
        } else {
            int nullX = drawX + NODE_W + 6;
            int nullY = START_Y + NODE_H / 2;
            QColor nullColor("#a090b8");
            nullColor.setAlphaF(alpha);
            p.setPen(QPen(nullColor, 1, Qt::DashLine));
            p.drawLine(drawX + NODE_W, nullY, nullX + 20, nullY);
            p.setPen(nullColor);
            p.setFont(QFont("Segoe UI", 9, QFont::StyleItalic));
            p.drawText(nullX + 22, nullY + 4, "nullptr");
        }

        x += NODE_W + GAP;
        curr = curr->next;
        i++;
    }
}
