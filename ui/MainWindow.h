#pragma once
#include <QDialog>
#include <QTimer>
#include "../logic/LinkedList.h"
#include "../logic/Stack.h"
#include "../logic/Queue.h"
#include "../logic/DoubleLinkedList.h"
#include "LinkedListWidget.h"
#include "StackWidget.h"
#include "QueueWidget.h"
#include "DLLWidget.h"
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

    // Lista Doblemente Enlazada
    void on_dllBtnInsertHead_clicked();
    void on_dllBtnInsertTail_clicked();
    void on_dllBtnInsertAt_clicked();
    void on_dllBtnRemove_clicked();
    void on_dllBtnSearch_clicked();
    void on_dllBtnClear_clicked();

    // Persistencia
    void on_btnSave_clicked();
    void on_btnLoad_clicked();

private:
    Ui::MainWindow* ui;

    // Estructuras lógicas
    LinkedList* list;
    Stack* stack;
    Queue* queue;
    DoubleLinkedList* dll;

    // Helpers
    void refreshLL();
    void refreshStack();
    void refreshQueue();
    void refreshDLL();

    void setLLStatus (const QString& msg, bool ok = true);
    void setStackStatus(const QString& msg, bool ok = true);
    void setQueueStatus(const QString& msg, bool ok = true);
    void setDLLStatus (const QString& msg, bool ok = true);
};
