#include "BSTWidget.h"
#include <QPainter>
#include <QFont>
#include <cmath>

static const QColor BST_BG ("#fff0f5");
static const QColor BST_ROOT_FILL ("#e8559a");
static const QColor BST_NODE_FILL ("#f4a0c8");
static const QColor BST_ROOT_BORDER ("#a0205a");
static const QColor BST_NODE_BORDER ("#c05080");
static const QColor BST_EDGE ("#c05080");
static const QColor BST_TEXT ("#ffffff");
static const QColor BST_META ("#802050");
static const QColor BST_ANIM_FILL ("#ff66aa");
static const QColor BST_ANIM_BORDER ("#cc0055");
static const QColor BST_DEL_FILL ("#ff4444");
static const QColor BST_DEL_BORDER ("#990000");
static const QColor BST_SUCC_FILL ("#ff9900");
static const QColor BST_SUCC_BORDER ("#cc6600");
static const QColor BST_HL_FILL ("#ffe066");
static const QColor BST_HL_BORDER ("#c8a800");

BSTWidget::BSTWidget(QWidget* parent)
    : QWidget(parent), bst(nullptr),
    highlightedVal(-1), highlightActive(false),
    animState(ANIM_NONE), animSteps(0), animCurrent(-1),
    deleteTarget(-1), deleteSuccessor(-1)
{
    setMinimumHeight(380);
    setMinimumWidth(600);
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &BSTWidget::animationStep);
}

void BSTWidget::setTree(BST* tree) {
    bst = tree;
    update();
}

void BSTWidget::highlightNode(int val) {
    highlightedVal = val;
    highlightActive = true;
    update();
}

void BSTWidget::clearHighlight() {
    highlightActive = false;
    update();
}

// ANIMACIÓN DE INSERCIÓN
int BSTWidget::buildPath(BSTNode* root, int target, int* path) {
    int steps = 0;
    BSTNode* curr = root;
    while (curr) {
        path[steps++] = curr->value;
        if (curr->value == target) {
            break;
        }
        else if (target < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return steps;
}

void BSTWidget::animateInsertion(int val) {
    if (!bst || bst->isEmpty()) {
        return;
    }
    animState = ANIM_INSERT_PATH;
    animSteps = buildPath(bst->getRoot(), val, animVals);
    animCurrent = 0;
    animTimer->setInterval(400);
    animTimer->start();
    update();
}

// ANIMACIÓN DE ELIMINACIÓN
int BSTWidget::findSuccessorVal(BSTNode* root, int val) {
    BSTNode* curr = root;
    while (curr) {
        if (curr->value == val) {
            break;
        }
        else if (val < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    if (!curr || !curr->right) {
        return -1;
    }
    BSTNode* succ = curr->right;
    while (succ->left) {
        succ = succ->left;
    }
    return succ->value;
}

void BSTWidget::animateDeletion(int val) {
    if (!bst || !bst->search(val)) {
        return;
    }
    animState = ANIM_DELETE_TARGET;
    deleteTarget = val;
    deleteSuccessor = findSuccessorVal(bst->getRoot(), val);
    animTimer->setInterval(600);
    animTimer->start();
    update();
}

// ANIMATION STEP
void BSTWidget::animationStep() {
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
void BSTWidget::computePositions(BSTNode* node, int depth, int& counter,
                                 int* xPos, int* yPos, int* vals, int& total) {
    if (node == nullptr) {
        return;
    }
    computePositions(node->left,  depth + 1, counter, xPos, yPos, vals, total);
    int idx = total++;
    vals[idx] = node->value;
    xPos[idx] = counter * H_GAP;
    yPos[idx] = 50 + depth * V_GAP;
    counter++;
    computePositions(node->right, depth + 1, counter, xPos, yPos, vals, total);
}

bool BSTWidget::findPos(int val, int* vals, int* xPos, int* yPos,
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
void BSTWidget::drawEdges(QPainter& p, BSTNode* node,
                          int* vals, int* xPos, int* yPos, int total) {
    if (node == nullptr) {
        return;
    }

    int px, py;
    if (!findPos(node->value, vals, xPos, yPos, total, px, py)) {
        return;
    }

    p.setPen(QPen(BST_EDGE, 2));

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
void BSTWidget::drawNode(QPainter& p, BSTNode* node, int x, int y,
                         bool isHighlighted, bool isAnimPath,
                         bool isDeleteTarget, bool isDeleteSuccessor)
{
    bool isRoot = (node == bst->getRoot());

    QColor fill, border;
    if (isDeleteTarget) {
        fill = BST_DEL_FILL;
        border = BST_DEL_BORDER;
    } else if (isDeleteSuccessor) {
        fill = BST_SUCC_FILL;
        border = BST_SUCC_BORDER;
    } else if (isHighlighted) {
        fill = BST_HL_FILL;
        border = BST_HL_BORDER;
    } else if (isAnimPath) {
        fill = BST_ANIM_FILL;
        border = BST_ANIM_BORDER;
    } else if (isRoot) {
        fill = BST_ROOT_FILL;
        border = BST_ROOT_BORDER;
    } else {
        fill = BST_NODE_FILL;
        border = BST_NODE_BORDER;
    }

    if (isRoot) {
        p.setPen(BST_ROOT_BORDER);
        p.setFont(QFont("Segoe UI", 9, QFont::Bold));
        p.drawText(QRect(x - NODE_R, y - NODE_R - 20, NODE_R * 2, 18),
                   Qt::AlignCenter, "raíz");
    }

    p.setBrush(fill);
    p.setPen(QPen(border, 2.5));
    p.drawEllipse(QPoint(x, y), NODE_R, NODE_R);

    p.setPen(BST_TEXT);
    p.setFont(QFont("Segoe UI", 12, QFont::Bold));
    p.drawText(QRect(x - NODE_R, y - NODE_R, NODE_R * 2, NODE_R * 2),
               Qt::AlignCenter, QString::number(node->value));

    int h = bst->height(node);
    int d = bst->depth(bst->getRoot(), node->value);
    int b = bst->balanceFactor(node);

    QRect metaRect(x - 34, y + NODE_R + 8, 68, 13);
    p.setPen(BST_META);
    p.setFont(QFont("Segoe UI", 8));
    p.drawText(metaRect, Qt::AlignCenter,
               QString("h:%1 d:%2 b:%3").arg(h).arg(d).arg(b));
}

// PAINTEVENT
void BSTWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), BST_BG);

    if (!bst || bst->isEmpty()) {
        p.setPen(BST_META);
        p.setFont(QFont("Segoe UI", 11));
        p.drawText(rect(), Qt::AlignCenter, "Árbol vacío");
        return;
    }

    const int MAX = 128;
    int vals[MAX], xPos[MAX], yPos[MAX];
    int total = 0, counter = 0;
    computePositions(bst->getRoot(), 0, counter, xPos, yPos, vals, total);

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

    drawEdges(p, bst->getRoot(), vals, xPos, yPos, total);

    for (int i = 0; i < total; i++) {
        BSTNode* node = nullptr;
        BSTNode* curr = bst->getRoot();
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
