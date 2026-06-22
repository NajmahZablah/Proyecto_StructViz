#include "AVLWidget.h"
#include <QPainter>
#include <QFont>
#include <cmath>

static const QColor AVL_BG ("#fff5f5");
static const QColor AVL_ROOT_FILL ("#cc2222");
static const QColor AVL_NODE_FILL ("#f08080");
static const QColor AVL_ROOT_BORDER ("#880000");
static const QColor AVL_NODE_BORDER ("#aa3333");
static const QColor AVL_EDGE ("#aa3333");
static const QColor AVL_TEXT ("#ffffff");
static const QColor AVL_META_OK ("#880000");
static const QColor AVL_META_WARN ("#8a5000");
static const QColor AVL_ANIM_FILL ("#ff8888");
static const QColor AVL_ANIM_BORDER ("#cc0000");
static const QColor AVL_DEL_FILL ("#cc0000");
static const QColor AVL_DEL_BORDER ("#660000");
static const QColor AVL_SUCC_FILL ("#ff9900");
static const QColor AVL_SUCC_BORDER ("#cc6600");
static const QColor AVL_HL_FILL ("#ffe066");
static const QColor AVL_HL_BORDER ("#c8a800");

AVLWidget::AVLWidget(QWidget* parent)
    : QWidget(parent), avl(nullptr),
    highlightedVal(-1), highlightActive(false),
    animState(ANIM_NONE), animSteps(0), animCurrent(-1),
    deleteTarget(-1), deleteSuccessor(-1)
{
    setMinimumHeight(380);
    setMinimumWidth(600);
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &AVLWidget::animationStep);
}

void AVLWidget::setTree(AVL* tree) {
    avl = tree; update();
}

void AVLWidget::highlightNode(int val) {
    highlightedVal = val;
    highlightActive = true;
    update();
}

void AVLWidget::clearHighlight() {
    highlightActive = false;
    update();
}

// ANIMACIÓN DE INSERCIÓN
int AVLWidget::buildPath(AVLNode* root, int target, int* path) {
    int steps = 0;
    AVLNode* curr = root;
    while (curr) {
        path[steps++] = curr->value;
        if (curr->value == target) {
            break;
        } else if (target < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return steps;
}

void AVLWidget::animateInsertion(int val) {
    if (!avl || avl->isEmpty()) {
        return;
    }
    animState = ANIM_INSERT_PATH;
    animSteps = buildPath(avl->getRoot(), val, animVals);
    animCurrent = 0;
    animTimer->setInterval(400);
    animTimer->start();
    update();
}

// ANIMACIÓN DE ELIMINACIÓN
int AVLWidget::findSuccessorVal(AVLNode* root, int val) {
    AVLNode* curr = root;
    while (curr) {
        if (curr->value == val) {
            break;
        } else if (val < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    if (!curr || !curr->right) {
        return -1;
    }
    AVLNode* succ = curr->right;
    while (succ->left) {
        succ = succ->left;
    }
    return succ->value;
}

void AVLWidget::animateDeletion(int val) {
    if (!avl || !avl->search(val)) {
        return;
    }
    animState = ANIM_DELETE_TARGET;
    deleteTarget = val;
    deleteSuccessor = findSuccessorVal(avl->getRoot(), val);
    animTimer->setInterval(600);
    animTimer->start();
    update();
}

// ANIMATION STEP
void AVLWidget::animationStep() {
    switch (animState) {

    case ANIM_INSERT_PATH:
        animCurrent++;
        if (animCurrent >= animSteps) {
            animTimer->stop();
            animState = ANIM_NONE;
            emit insertionAnimationDone();
        }
        break;

    case ANIM_DELETE_TARGET:
        if (deleteSuccessor != -1) {
            animState = ANIM_DELETE_SUCCESSOR;
        } else {
            animState = ANIM_DELETE_DONE;
            animTimer->setInterval(300);
        }
        break;

    case ANIM_DELETE_SUCCESSOR:
        animState = ANIM_DELETE_DONE;
        animTimer->setInterval(300);
        break;

    case ANIM_DELETE_DONE:
        animTimer->stop();
        animState = ANIM_NONE;
        emit deletionAnimationDone(deleteTarget);
        deleteTarget    = -1;
        deleteSuccessor = -1;
        break;

    default:
        animTimer->stop();
        break;
    }
    update();
}

// COMPUTEPOSITIONS + FINDPOS
void AVLWidget::computePositions(AVLNode* node, int depth, int& counter,
                                 int* xPos, int* yPos, int* vals, int& total) {
    if (node == nullptr) {
        return;
    }
    computePositions(node->left, depth + 1, counter, xPos, yPos, vals, total);
    int idx = total++;
    vals[idx] = node->value;
    xPos[idx] = counter * H_GAP;
    yPos[idx] = 50 + depth * V_GAP;
    counter++;
    computePositions(node->right, depth + 1, counter, xPos, yPos, vals, total);
}

bool AVLWidget::findPos(int val, int* vals, int* xPos, int* yPos,
                        int total, int& outX, int& outY) {
    for (int i = 0; i < total; i++) {
        if (vals[i] == val) {
            outX = xPos[i];
            outY = yPos[i];
            return true;
        }
    }
    return false;
}

// DRAWEDGES
void AVLWidget::drawEdges(QPainter& p, AVLNode* node,
                          int* vals, int* xPos, int* yPos, int total)
{
    if (node == nullptr) {
        return;
    }
    int px, py;
    if (!findPos(node->value, vals, xPos, yPos, total, px, py)) {
        return;
    }

    p.setPen(QPen(AVL_EDGE, 2));

    auto edgePoint = [](int fx, int fy, int tx, int ty, int r, int& ox, int& oy) {
        double dx = tx - fx, dy = ty - fy;
        double len = sqrt(dx*dx + dy*dy);
        if (len < 0.001) {
            ox = fx;
            oy = fy;
            return;
        }
        ox = (int)(fx + dx / len * r);
        oy = (int)(fy + dy / len * r);
    };

    if (node->left) {
        int cx, cy;
        if (findPos(node->left->value, vals, xPos, yPos, total, cx, cy)) {
            int sx, sy, ex, ey;
            edgePoint(px, py, cx, cy, NODE_R + 2, sx, sy);
            edgePoint(cx, cy, px, py, NODE_R + 2, ex, ey);
            p.drawLine(sx, sy, ex, ey);
        }
        drawEdges(p, node->left, vals, xPos, yPos, total);
    }
    if (node->right) {
        int cx, cy;
        if (findPos(node->right->value, vals, xPos, yPos, total, cx, cy)) {
            int sx, sy, ex, ey;
            edgePoint(px, py, cx, cy, NODE_R + 2, sx, sy);
            edgePoint(cx, cy, px, py, NODE_R + 2, ex, ey);
            p.drawLine(sx, sy, ex, ey);
        }
        drawEdges(p, node->right, vals, xPos, yPos, total);
    }
}

// DRAWNODE
void AVLWidget::drawNode(QPainter& p, AVLNode* node, int x, int y,
                         bool isHighlighted, bool isAnimPath,
                         bool isDeleteTarget, bool isDeleteSuccessor) {
    bool isRoot = (node == avl->getRoot());
    int  balance = avl->getBalance(node);

    QColor fill, border;
    if (isDeleteTarget) {
        fill = AVL_DEL_FILL;
        border = AVL_DEL_BORDER;
    } else if (isDeleteSuccessor) {
        fill = AVL_SUCC_FILL;
        border = AVL_SUCC_BORDER;
    } else if (isHighlighted) {
        fill = AVL_HL_FILL;
        border = AVL_HL_BORDER;
    } else if (isAnimPath) {
        fill = AVL_ANIM_FILL;
        border = AVL_ANIM_BORDER;
    } else if (isRoot) {
        fill = AVL_ROOT_FILL;
        border = AVL_ROOT_BORDER;
    } else {
        fill = AVL_NODE_FILL;
        border = AVL_NODE_BORDER;
    }

    if (isRoot) {
        p.setPen(AVL_ROOT_BORDER);
        p.setFont(QFont("Segoe UI", 9, QFont::Bold));
        p.drawText(QRect(x - NODE_R, y - NODE_R - 20, NODE_R * 2, 18),
                   Qt::AlignCenter, "raíz");
    }

    p.setBrush(fill);
    p.setPen(QPen(border, 2.5));
    p.drawEllipse(QPoint(x, y), NODE_R, NODE_R);

    p.setPen(AVL_TEXT);
    p.setFont(QFont("Segoe UI", 12, QFont::Bold));
    p.drawText(QRect(x - NODE_R, y - NODE_R, NODE_R * 2, NODE_R * 2),
               Qt::AlignCenter, QString::number(node->value));

    int h = node->height - 1;
    int d = avl->depth(avl->getRoot(), node->value);

    QRect hdRect(x - 30, y + NODE_R + 8, 38, 13);
    p.setPen(AVL_META_OK);
    p.setFont(QFont("Segoe UI", 8));
    p.drawText(hdRect, Qt::AlignLeft, QString("h:%1 d:%2").arg(h).arg(d));

    QColor bColor = (balance == 0) ? AVL_META_OK
                    : (balance == 1 || balance == -1) ? AVL_META_WARN
                                                      : QColor("#cc0000");

    QRect bRect(x + 10, y + NODE_R + 8, 22, 13);
    p.setPen(bColor);
    p.setFont(QFont("Segoe UI", 8, QFont::Bold));
    p.drawText(bRect, Qt::AlignLeft, QString("b:%1").arg(balance));
}

// PAINTEVENT
void AVLWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), AVL_BG);

    if (!avl || avl->isEmpty()) {
        p.setPen(AVL_META_OK);
        p.setFont(QFont("Segoe UI", 11));
        p.drawText(rect(), Qt::AlignCenter, "Árbol vacío");
        return;
    }

    const int MAX = 128;
    int vals[MAX], xPos[MAX], yPos[MAX];
    int total = 0, counter = 0;
    computePositions(avl->getRoot(), 0, counter, xPos, yPos, vals, total);

    int minX = xPos[0], maxX = xPos[0];
    for (int i = 1; i < total; i++) {
        if (xPos[i] < minX) {
            minX = xPos[i];
        }

        if (xPos[i] > maxX) {
            maxX = xPos[i];
        }
    }
    int offsetX = (width() - (maxX - minX + NODE_R * 2)) / 2 - minX + NODE_R;
    for (int i = 0; i < total; i++) {
        xPos[i] += offsetX;
    }

    drawEdges(p, avl->getRoot(), vals, xPos, yPos, total);

    for (int i = 0; i < total; i++) {
        AVLNode* node = nullptr;
        AVLNode* curr = avl->getRoot();
        while (curr) {
            if (curr->value == vals[i]) {
                node = curr;
                break;
            } else if (vals[i] < curr->value) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        if (!node) {
            continue;
        }

        bool isAnimPath = false;
        if (animState == ANIM_INSERT_PATH && animCurrent >= 0) {
            for (int s = 0; s <= animCurrent && s < animSteps; s++) {
                if (animVals[s] == vals[i]) {
                    isAnimPath = true;
                    break;
                }
            }
        }

        bool isDelTarget = (animState == ANIM_DELETE_TARGET ||
                            animState == ANIM_DELETE_SUCCESSOR ||
                            animState == ANIM_DELETE_DONE)
                           && (node->value == deleteTarget);

        bool isDelSucc = (animState == ANIM_DELETE_SUCCESSOR)
                         && (node->value == deleteSuccessor);

        bool isHL = highlightActive && (node->value == highlightedVal);

        drawNode(p, node, xPos[i], yPos[i], isHL, isAnimPath, isDelTarget, isDelSucc);
    }
}
