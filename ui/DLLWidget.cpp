#include "DLLWidget.h"
#include <QPainter>
#include <QFont>

// CONSTRUCTOR
DLLWidget::DLLWidget(QWidget* parent)
    : QWidget(parent), dll(nullptr), highlightedVal(-1), highlightActive(false)
{
    setMinimumHeight(140);
}

// SETLIST
void DLLWidget::setList(DoubleLinkedList* list) {
    dll = list;
    update();
}

// HIGHLIGHT
void DLLWidget::highlightNode(int val) {
    highlightedVal = val;
    highlightActive = true;
    update();
}

void DLLWidget::clearHighlight() {
    highlightActive = false;
    update();
}

// PAINTEVENT
void DLLWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Fondo
    p.fillRect(rect(), QColor("#fff5f0"));

    if (!dll || dll->isEmpty()) {
        p.setPen(QColor("#c08070"));
        p.setFont(QFont("Segoe UI", 11));
        p.drawText(rect(), Qt::AlignCenter, "Lista vacía  ⟷  nullptr");
        return;
    }

    DNode* curr = dll->getHead();
    int x = START_X;

    // Badge "nullptr" izquierdo
    {
        int badgeW = 52;
        int badgeH = 20;
        int badgeX = x - badgeW - 6;
        int badgeY = START_Y + NODE_H / 2 - 14;
        int arrowY = START_Y + NODE_H / 2 - 6;

        p.setBrush(QColor("#f8e0d8"));
        p.setPen(QPen(QColor("#d09080"), 1.5));
        p.drawRoundedRect(badgeX, badgeY, badgeW, badgeH, 6, 6);
        p.setPen(QColor("#a06050"));
        p.setFont(QFont("Segoe UI", 8, QFont::StyleItalic));
        p.drawText(QRect(badgeX, badgeY, badgeW, badgeH), Qt::AlignCenter, "nullptr");


        // Flecha desde head hacia el badge (head->prev apunta a nullptr)
        QColor arrowColor("#c07060");
        p.setPen(QPen(arrowColor, 4, Qt::SolidLine, Qt::RoundCap));
        p.drawLine(x - 4, arrowY, badgeX + badgeW + 11, arrowY);

        QPolygon arr;
        arr << QPoint(badgeX + badgeW, arrowY)
            << QPoint(badgeX + badgeW + 11, arrowY - 5)
            << QPoint(badgeX + badgeW + 11, arrowY + 5);
        p.setBrush(arrowColor);
        p.setPen(Qt::NoPen);
        p.drawPolygon(arr);
    }

    while (curr) {
        bool isHead = (curr == dll->getHead());
        bool isTail = (curr == dll->getTail());
        bool isHighlighted = (highlightActive && curr->value == highlightedVal);

        // Colores - paleta monocromática naranja
        QColor nodeColor = isHighlighted ? QColor("#f0a0b8")
                           : isHead ? QColor("#e8822a")
                           : isTail ? QColor("#fde0c8")
                                    : QColor("#f5b478");
        QColor borderColor = isHighlighted ? QColor("#d06080")
                             : isHead ? QColor("#b85a10")
                             : isTail ? QColor("#e0a060")
                                      : QColor("#c87828");
        QColor textColor = isHighlighted ? QColor("#803050")
                           : isHead ? QColor("#ffffff")
                           : isTail ? QColor("#804020")
                                    : QColor("#ffffff");

        // Etiqueta head / tail / head+tail (nodo único)
        if (isHead || isTail) {
            bool isBoth = isHead && isTail;
            QColor labelColor = isBoth ? QColor("#804030")
                                : isHead ? QColor("#a05020")
                                         : QColor("#307060");
            p.setPen(labelColor);
            p.setFont(QFont("Segoe UI", 8, QFont::Bold));
            p.drawText(QRect(x, START_Y - 22, NODE_W, 16),
                       Qt::AlignCenter,
                       isBoth ? "head/tail" : isHead ? "head" : "tail");
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

        // Flechas bidireccionales o badge nullptr
        if (curr->next) {
            int midY = START_Y + NODE_H / 2;
            int nextY = midY - 6;
            int prevY = midY + 6;
            int startX = x + NODE_W + 4;
            int endX = x + NODE_W + GAP - 4;

            QColor arrowColor("#c07060");

            // Flecha next -> (izquierda a derecha, arriba)
            p.setPen(QPen(arrowColor, 4, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(startX, nextY, endX - 11, nextY);
            QPolygon arrNext;
            arrNext << QPoint(endX, nextY)
                    << QPoint(endX - 11, nextY - 5)
                    << QPoint(endX - 11, nextY + 5);
            p.setBrush(arrowColor);
            p.setPen(Qt::NoPen);
            p.drawPolygon(arrNext);

            // Flecha prev <- (derecha a izquierda, abajo)
            p.setPen(QPen(arrowColor, 4, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(startX + 11, prevY, endX, prevY);
            QPolygon arrPrev;
            arrPrev << QPoint(startX, prevY)
                    << QPoint(startX + 11, prevY - 5)
                    << QPoint(startX + 11, prevY + 5);
            p.setBrush(arrowColor);
            p.setPen(Qt::NoPen);
            p.drawPolygon(arrPrev);

        } else {
            // Badge "nullptr" derecho - después del tail
            int arrowY = START_Y + NODE_H / 2 - 6;
            int badgeX = x + NODE_W + 8;
            int badgeY = START_Y + NODE_H / 2 - 14;
            int badgeW = 52;
            int badgeH = 20;

            QColor arrowColor("#c07060");

            // Línea next hacia el badge
            p.setPen(QPen(arrowColor, 4, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(x + NODE_W + 4, arrowY, badgeX, arrowY);

            // Badge
            p.setBrush(QColor("#f8e0d8"));
            p.setPen(QPen(QColor("#d09080"), 1.5));
            p.drawRoundedRect(badgeX, badgeY, badgeW, badgeH, 6, 6);
            p.setPen(QColor("#a06050"));
            p.setFont(QFont("Segoe UI", 8, QFont::StyleItalic));
            p.drawText(QRect(badgeX, badgeY, badgeW, badgeH),
                       Qt::AlignCenter, "nullptr");
        }

        x += NODE_W + GAP;
        curr = curr->next;
    }
}
