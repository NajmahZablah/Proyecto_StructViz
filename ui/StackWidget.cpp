#include "StackWidget.h"
#include <QPainter>
#include <QFont>

// CONSTRUCTOR
StackWidget::StackWidget(QWidget* parent)
    : QWidget(parent), st(nullptr), highlightedVal(-1), highlightActive(false)
{
    setMinimumHeight(200);
}

// SETSTACK
void StackWidget::setStack(Stack* stack) {
    st = stack;
    update();
}

// HIGHLIGHT
void StackWidget::highlightNode(int val) {
    highlightedVal = val;
    highlightActive = true;
    update();
}

void StackWidget::clearHighlight() {
    highlightActive = false;
    update();
}

// PAINTEVENT
void StackWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Fondo
    p.fillRect(rect(), QColor("#f0f4ff"));

    if (!st || st->isEmpty()) {
        p.setPen(QColor("#8090c0"));
        p.setFont(QFont("Segoe UI", 11));
        p.drawText(rect(), Qt::AlignCenter, "Stack vacío  ↓  nullptr");
        return;
    }

    Node* curr = st->getTop();
    int y = START_Y;
    int i = 0;

    while (curr) {
        bool isTop = (i == 0);
        bool isBase = (curr->next == nullptr);
        bool isHighlighted = (highlightActive && curr->value == highlightedVal);

        // Colores - paleta monocromática azul
        QColor nodeColor = isHighlighted ? QColor("#f0a0b8")
                           : isTop ? QColor("#5a7ed0")
                           : isBase ? QColor("#c8d8f8")
                                    : QColor("#8aaae8");
        QColor borderColor = isHighlighted ? QColor("#d06080")
                             : isTop ? QColor("#3050a8")
                             : isBase ? QColor("#7090d0")
                                      : QColor("#4868b8");
        QColor textColor = isHighlighted ? QColor("#803050")
                           : isTop ? QColor("#ffffff")
                           : isBase ? QColor("#303890")
                                    : QColor("#ffffff");

        // Etiqueta top / base / top+base (nodo único)
        if (isTop || isBase) {
            bool isBoth = isTop && isBase;
            QColor labelColor = isBoth ? QColor("#354090")
                                : isTop ? QColor("#4050a0")
                                        : QColor("#307060");
            p.setPen(labelColor);
            p.setFont(QFont("Segoe UI", 8, QFont::Bold));
            p.drawText(QRect(START_X + NODE_W + 8, y + NODE_H / 4, 60, 16),
                       Qt::AlignLeft,
                       isBoth ? "top/base" : isTop ? "top" : "base");
        }

        // Nodo
        p.setBrush(nodeColor);
        p.setPen(QPen(borderColor, 2));
        p.drawRoundedRect(START_X, y, NODE_W, NODE_H, 10, 10);

        // Valor
        p.setPen(textColor);
        p.setFont(QFont("Segoe UI", 13, QFont::Bold));
        p.drawText(QRect(START_X, y, NODE_W, NODE_H),
                   Qt::AlignCenter,
                   QString::number(curr->value));

        // Flecha hacia abajo o badge nullptr
        if (curr->next) {
            int arrowX = START_X + NODE_W / 2;
            int arrowStartY = y + NODE_H + 4;
            int arrowEndY = y + NODE_H + GAP - 4;

            QColor arrowColor("#5060b0");
            p.setPen(QPen(arrowColor, 4, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(arrowX, arrowStartY, arrowX, arrowEndY - 11);

            QPolygon arrow;
            arrow << QPoint(arrowX, arrowEndY)
                  << QPoint(arrowX - 6, arrowEndY - 11)
                  << QPoint(arrowX + 6, arrowEndY - 11);
            p.setBrush(arrowColor);
            p.setPen(Qt::NoPen);
            p.drawPolygon(arrow);

        } else {
            int arrowX = START_X + NODE_W / 2;
            int badgeX = START_X + NODE_W / 2 - 26;
            int badgeY = y + NODE_H + 10;
            int badgeW = 52;
            int badgeH = 20;

            QColor arrowColor("#5060b0");
            p.setPen(QPen(arrowColor, 4, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(arrowX, y + NODE_H + 4, arrowX, badgeY);

            // Badge
            p.setBrush(QColor("#d8ddf8"));
            p.setPen(QPen(QColor("#6878d0"), 1.5));
            p.drawRoundedRect(badgeX, badgeY, badgeW, badgeH, 6, 6);

            p.setPen(QColor("#404880"));
            p.setFont(QFont("Segoe UI", 8, QFont::StyleItalic));
            p.drawText(QRect(badgeX, badgeY, badgeW, badgeH),
                       Qt::AlignCenter, "nullptr");
        }

        y += NODE_H + GAP;
        curr = curr->next;
        i++;
    }
}
