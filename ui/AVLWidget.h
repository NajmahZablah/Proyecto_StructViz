#pragma once
#include <QWidget>
#include <QTimer>
#include "../logic/AVL.h"

// WIDGET DE VISUALIZACIÓN DEL AVL
class AVLWidget : public QWidget {
    Q_OBJECT
public:
    explicit AVLWidget(QWidget* parent = nullptr);
    void setTree(AVL* tree);
    void highlightNode(int val);
    void clearHighlight();

    void animateInsertion(int val);
    void animateDeletion(int val);

signals:
    void insertionAnimationDone();
    void deletionAnimationDone(int val);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void animationStep();

private:
    AVL* avl;
    int  highlightedVal;
    bool highlightActive;

    enum AnimState {
        ANIM_NONE,
        ANIM_INSERT_PATH,
        ANIM_DELETE_TARGET,
        ANIM_DELETE_SUCCESSOR,
        ANIM_DELETE_DONE
    };
    AnimState animState;

    QTimer* animTimer;

    int animVals[128];
    int animSteps;
    int animCurrent;
    int deleteTarget;
    int deleteSuccessor;

    static const int NODE_R = 26;
    static const int H_GAP = 70;
    static const int V_GAP = 80;

    void computePositions(AVLNode* node, int depth, int& counter,
                          int* xPos, int* yPos, int* vals, int& total);

    void drawEdges(QPainter& p, AVLNode* node, int* vals,
                   int* xPos, int* yPos, int total);

    void drawNode(QPainter& p, AVLNode* node, int x, int y,
                  bool isHighlighted, bool isAnimPath,
                  bool isDeleteTarget, bool isDeleteSuccessor);

    bool findPos(int val, int* vals, int* xPos, int* yPos,
                 int total, int& outX, int& outY);

    int  buildPath(AVLNode* root, int target, int* path);

    int  findSuccessorVal(AVLNode* root, int val);
};
