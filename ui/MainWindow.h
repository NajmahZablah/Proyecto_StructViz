#pragma once
#include <QDialog>
#include <QTimer>
#include "../logic/LinkedList.h"
#include "../logic/Stack.h"
#include "../logic/Queue.h"
#include "../logic/DoubleLinkedList.h"
#include "../logic/BST.h"
#include "../logic/AVL.h"
#include "LinkedListWidget.h"
#include "StackWidget.h"
#include "QueueWidget.h"
#include "DLLWidget.h"
#include "BSTWidget.h"
#include "AVLWidget.h"
#include "../logic/Persistence.h"
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // Lista Enlazada
    void on_llBtnInsertHead_clicked();
    void on_llBtnInsertTail_clicked();
    void on_llBtnInsertAt_clicked();
    void on_llBtnRemove_clicked();
    void on_llBtnSearch_clicked();
    void on_llBtnClear_clicked();

    // Stack
    void on_stackBtnPush_clicked();
    void on_stackBtnPop_clicked();
    void on_stackBtnPeek_clicked();
    void on_stackBtnClear_clicked();

    // Queue
    void on_queueBtnEnqueue_clicked();
    void on_queueBtnDequeue_clicked();
    void on_queueBtnPeek_clicked();
    void on_queueBtnRear_clicked();
    void on_queueBtnClear_clicked();

    // Double Linked List
    void on_dllBtnInsertHead_clicked();
    void on_dllBtnInsertTail_clicked();
    void on_dllBtnInsertAt_clicked();
    void on_dllBtnRemove_clicked();
    void on_dllBtnSearch_clicked();
    void on_dllBtnClear_clicked();

    // BST
    void on_bstBtnInsert_clicked();
    void on_bstBtnRemove_clicked();
    void on_bstBtnSearch_clicked();
    void on_bstBtnClear_clicked();
    void on_bstBtnInorder_clicked();
    void on_bstBtnPreorder_clicked();
    void on_bstBtnPostorder_clicked();
    void on_bstBtnSuccessor_clicked();
    void on_bstBtnPredecessor_clicked();
    void on_bstBtnConvert_clicked();
    void onBSTDeletionDone(int val);

    // AVL
    void on_avlBtnInsert_clicked();
    void on_avlBtnRemove_clicked();
    void on_avlBtnSearch_clicked();
    void on_avlBtnClear_clicked();
    void on_avlBtnInorder_clicked();
    void on_avlBtnPreorder_clicked();
    void on_avlBtnPostorder_clicked();
    void on_avlBtnSuccessor_clicked();
    void on_avlBtnPredecessor_clicked();
    void onAVLDeletionDone(int val);

    // Conversión animada de BST a AVL
    void conversionStep();
    void onAVLInsertionDone();

    // Persistencia
    void on_btnSave_clicked();
    void on_btnLoad_clicked();

private:
    Ui::MainWindow* ui;

    LinkedList* list;
    Stack* stack;
    Queue* queue;
    DoubleLinkedList* dll;
    BST* bst;
    AVL* avl;

    // Conversión animada de BST a AVL
    QTimer* conversionTimer;
    int conversionVals[128];
    int conversionTotal;
    int conversionIdx;
    int conversionExpectedDepth;

    void refreshLL();
    void refreshStack();
    void refreshQueue();
    void refreshDLL();
    void refreshBST();
    void refreshAVL();

    void setLLStatus(const QString& msg, bool ok = true);
    void setStackStatus(const QString& msg, bool ok = true);
    void setQueueStatus(const QString& msg, bool ok = true);
    void setDLLStatus(const QString& msg, bool ok = true);
    void setBSTStatus(const QString& msg, bool ok = true);
    void setAVLStatus(const QString& msg, bool ok = true);

    QString traversalToString(int* arr, int size);
};
