#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTimer>

// CONSTRUCTOR
MainWindow::MainWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
    , list(new LinkedList())
    , stack(new Stack())
    , queue(new Queue())
    , dll(new DoubleLinkedList())
    , bst(new BST())
    , avl(new AVL())
    , conversionTotal(0)
    , conversionIdx(0)
{
    ui->setupUi(this);
    setWindowTitle("StructViz");

    // Timer para la conversión animada de BST a AVL (un nodo cada 600ms)
    conversionTimer = new QTimer(this);
    conversionTimer->setInterval(600);
    connect(conversionTimer, &QTimer::timeout, this, &MainWindow::conversionStep);

    // Conectar señales de animación de los widgets
    BSTWidget* bstW = qobject_cast<BSTWidget*>(ui->bstDisplay);
    if (bstW) {
        connect(bstW, &BSTWidget::deletionAnimationDone,
                this, &MainWindow::onBSTDeletionDone);
        connect(bstW, &BSTWidget::insertionAnimationDone,
                this, [this]() { refreshBST(); });
    }

    AVLWidget* avlW = qobject_cast<AVLWidget*>(ui->avlDisplay);
    if (avlW) {
        connect(avlW, &AVLWidget::deletionAnimationDone,
                this, &MainWindow::onAVLDeletionDone);
        connect(avlW, &AVLWidget::insertionAnimationDone,
                this, &MainWindow::onAVLInsertionDone);
    }

    setStyleSheet(R"(
        QDialog {
            background-color: #ede6f5;
        }
        QTabWidget::pane {
            border: 1.5px solid #c0a8e0;
            border-radius: 10px;
            background-color: #f5f0ff;
        }
        QTabBar::tab {
            background-color: #e0d4f5;
            border: 1.5px solid #c0a8e0;
            border-bottom: none;
            border-radius: 8px 8px 0 0;
            padding: 6px 18px;
            color: #6a4f90;
            font-family: 'Segoe UI';
            font-weight: bold;
        }
        QTabBar::tab:selected {
            background-color: #f5f0ff;
            color: #4a2f70;
        }
        QTabBar::tab:hover {
            background-color: #ece0ff;
        }
        QLabel#lblTitulo {
            color: #7a5fa0;
            font-size: 20px;
            font-weight: bold;
            font-family: 'Segoe UI';
        }
        QLabel {
            color: #8a70b0;
            font-size: 12px;
            font-family: 'Segoe UI';
        }
        QLineEdit {
            background-color: #ede0ff;
            border: 1.5px solid #c0a8e0;
            border-radius: 10px;
            padding: 6px 12px;
            color: #6a4f90;
            font-size: 13px;
            font-family: 'Segoe UI';
        }
        QLineEdit:focus {
            border: 1.5px solid #a888d0;
            background-color: #e8d8ff;
        }
        QPushButton {
            background-color: #e8d8f8;
            border: 1.5px solid #c0a8e0;
            border-radius: 10px;
            color: #5a3f8a;
            font-family: 'Segoe UI';
            font-weight: bold;
            padding: 6px 12px;
        }
        QPushButton:hover {
            background-color: #dccaf5;
        }
        QPushButton:pressed {
            padding: 7px 11px 5px 13px;
        }
    )");

    refreshLL();
    refreshStack();
    refreshQueue();
    refreshDLL();
    refreshBST();
    refreshAVL();
}

// DESTRUCTOR
MainWindow::~MainWindow() {
    delete ui;
    delete list;
    delete stack;
    delete queue;
    delete dll;
    delete bst;
    delete avl;
}

// HELPERS
void MainWindow::refreshLL() {
    LinkedListWidget* w = qobject_cast<LinkedListWidget*>(ui->llDisplay);
    if (w) {
        w->setList(list);
    }
    ui->llSize->setText(QString("Tamaño: %1 nodo%2")
                            .arg(list->size())
                            .arg(list->size() == 1 ? "" : "s"));
}

void MainWindow::refreshStack() {
    StackWidget* w = qobject_cast<StackWidget*>(ui->stackDisplay);
    if (w) {
        w->setStack(stack);
    }
    ui->stackSize->setText(QString("Tamaño: %1 nodo%2")
                               .arg(stack->size())
                               .arg(stack->size() == 1 ? "" : "s"));
}

void MainWindow::refreshQueue() {
    QueueWidget* w = qobject_cast<QueueWidget*>(ui->queueDisplay);
    if (w) {
        w->setQueue(queue);
    }
    ui->queueSize->setText(QString("Tamaño: %1 nodo%2")
                               .arg(queue->size())
                               .arg(queue->size() == 1 ? "" : "s"));
}

void MainWindow::refreshDLL() {
    DLLWidget* w = qobject_cast<DLLWidget*>(ui->dllDisplay);
    if (w) {
        w->setList(dll);
    }
    ui->dllSize->setText(QString("Tamaño: %1 nodo%2")
                             .arg(dll->size())
                             .arg(dll->size() == 1 ? "" : "s"));
}

void MainWindow::refreshBST() {
    BSTWidget* w = qobject_cast<BSTWidget*>(ui->bstDisplay);
    if (w) {
        w->setTree(bst);
    }
    ui->bstSize->setText(QString("Tamaño: %1 nodo%2")
                             .arg(bst->size())
                             .arg(bst->size() == 1 ? "" : "s"));
}

void MainWindow::refreshAVL() {
    AVLWidget* w = qobject_cast<AVLWidget*>(ui->avlDisplay);
    if (w) {
        w->setTree(avl);
    }
    ui->avlSize->setText(QString("Tamaño: %1 nodo%2")
                             .arg(avl->size())
                             .arg(avl->size() == 1 ? "" : "s"));
}

// Status helpers
void MainWindow::setLLStatus(const QString& msg, bool ok) {
    ui->llStatus->setText(msg);
    ui->llStatus->setStyleSheet(ok ? "color: #5a8a5a; font-weight: bold;"
                                   : "color: #a03040; font-weight: bold;");
}
void MainWindow::setStackStatus(const QString& msg, bool ok) {
    ui->stackStatus->setText(msg);
    ui->stackStatus->setStyleSheet(ok ? "color: #5a8a5a; font-weight: bold;"
                                      : "color: #a03040; font-weight: bold;");
}
void MainWindow::setQueueStatus(const QString& msg, bool ok) {
    ui->queueStatus->setText(msg);
    ui->queueStatus->setStyleSheet(ok ? "color: #5a8a5a; font-weight: bold;"
                                      : "color: #a03040; font-weight: bold;");
}
void MainWindow::setDLLStatus(const QString& msg, bool ok) {
    ui->dllStatus->setText(msg);
    ui->dllStatus->setStyleSheet(ok ? "color: #5a8a5a; font-weight: bold;"
                                    : "color: #a03040; font-weight: bold;");
}
void MainWindow::setBSTStatus(const QString& msg, bool ok) {
    ui->bstStatus->setText(msg);
    ui->bstStatus->setStyleSheet(ok ? "color: #5a8a5a; font-weight: bold;"
                                    : "color: #a03040; font-weight: bold;");
}
void MainWindow::setAVLStatus(const QString& msg, bool ok) {
    ui->avlStatus->setText(msg);
    ui->avlStatus->setStyleSheet(ok ? "color: #5a8a5a; font-weight: bold;"
                                    : "color: #a03040; font-weight: bold;");
}

// Convierte el arreglo de un recorrido a un QString legible
QString MainWindow::traversalToString(int* arr, int size) {
    QString result;
    for (int i = 0; i < size; i++) {
        result += QString::number(arr[i]);
        if (i < size - 1) {
            result += " → ";
        }
    }
    return result;
}

// SLOTS: LISTA ENLAZADA
void MainWindow::on_llBtnInsertHead_clicked() {
    bool ok;
    int val = ui->llInputValue->text().toInt(&ok);
    if (!ok) {
        setLLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    list->insertAtHead(val);
    refreshLL();
    setLLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 insertado al inicio.").arg(val));
    ui->llInputValue->clear();
}

void MainWindow::on_llBtnInsertTail_clicked() {
    bool ok;
    int val = ui->llInputValue->text().toInt(&ok);
    if (!ok) {
        setLLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    list->insertAtTail(val);
    refreshLL();
    setLLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 insertado al final.").arg(val));
    ui->llInputValue->clear();
}

void MainWindow::on_llBtnInsertAt_clicked() {
    bool okVal, okIdx;
    int val = ui->llInputValue->text().toInt(&okVal);
    int idx = ui->llInputIndex->text().toInt(&okIdx);
    if (!okVal || !okIdx) {
        setLLStatus("( ˶°ㅁ°) !! Ingresa valor e índice válidos.", false);
        return;
    }
    list->insertAt(idx, val);
    refreshLL();
    setLLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 insertado en posición %2.").arg(val).arg(idx));
    ui->llInputValue->clear();
    ui->llInputIndex->clear();
}

void MainWindow::on_llBtnRemove_clicked() {
    bool ok;
    int val = ui->llInputValue->text().toInt(&ok);
    if (!ok) {
        setLLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    if (list->remove(val)) {
        refreshLL();
        setLLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ Nodo %1 eliminado.").arg(val));
    } else {
        setLLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
    }
    ui->llInputValue->clear();
}

void MainWindow::on_llBtnSearch_clicked() {
    bool ok;
    int val = ui->llInputValue->text().toInt(&ok);
    if (!ok) {
        setLLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    LinkedListWidget* w = qobject_cast<LinkedListWidget*>(ui->llDisplay);
    if (list->search(val)) {
        if (w) {
            w->highlightNode(val);
        }
        setLLStatus(QString("🔍 Valor %1 encontrado.").arg(val));
        QTimer::singleShot(2000, this, [w]() {
            if (w) w->clearHighlight();
        });
    } else {
        setLLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
    }
}

void MainWindow::on_llBtnClear_clicked() {
    list->clear();
    refreshLL();
    setLLStatus("Lista limpiada.");
}

// SLOTS: STACK
void MainWindow::on_stackBtnPush_clicked() {
    bool ok;
    int val = ui->stackInputValue->text().toInt(&ok);
    if (!ok) {
        setStackStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    stack->push(val);
    refreshStack();
    setStackStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 pushed al top.").arg(val));
    ui->stackInputValue->clear();
}

void MainWindow::on_stackBtnPop_clicked() {
    if (stack->isEmpty()) {
        setStackStatus("( ˶°ㅁ°) !! El stack está vacío.", false);
        return;
    }
    int val = stack->pop();
    refreshStack();
    setStackStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 removido del top.").arg(val));
}

void MainWindow::on_stackBtnPeek_clicked() {
    if (stack->isEmpty()) {
        setStackStatus("( ˶°ㅁ°) !! El stack está vacío.", false);
        return;
    }
    int val = stack->peek();
    StackWidget* w = qobject_cast<StackWidget*>(ui->stackDisplay);
    if (w) {
        w->highlightNode(val);
    }
    setStackStatus(QString("🔍 Top: %1").arg(val));
    QTimer::singleShot(2000, this, [w]() {
        if (w) w->clearHighlight();
    });
}

void MainWindow::on_stackBtnClear_clicked() {
    stack->clear();
    refreshStack();
    setStackStatus("Stack limpiado.");
}

// SLOTS: QUEUE
void MainWindow::on_queueBtnEnqueue_clicked() {
    bool ok;
    int val = ui->queueInputValue->text().toInt(&ok);
    if (!ok) {
        setQueueStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    queue->enqueue(val);
    refreshQueue();
    setQueueStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 agregado al back.").arg(val));
    ui->queueInputValue->clear();
}

void MainWindow::on_queueBtnDequeue_clicked() {
    if (queue->isEmpty()) {
        setQueueStatus("( ˶°ㅁ°) !! La queue está vacía.", false);
        return;
    }
    int val = queue->dequeue();
    refreshQueue();
    setQueueStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 removido del front.").arg(val));
}

void MainWindow::on_queueBtnPeek_clicked() {
    if (queue->isEmpty()) {
        setQueueStatus("( ˶°ㅁ°) !! La queue está vacía.", false);
        return;
    }
    int val = queue->peek();
    QueueWidget* w = qobject_cast<QueueWidget*>(ui->queueDisplay);
    if (w) {
        w->highlightNode(val);
    }
    setQueueStatus(QString("🔍 Front: %1").arg(val));
    QTimer::singleShot(2000, this, [w]() {
        if (w) w->clearHighlight();
    });
}

void MainWindow::on_queueBtnRear_clicked() {
    if (queue->isEmpty()) {
        setQueueStatus("( ˶°ㅁ°) !! La queue está vacía.", false);
        return;
    }
    int val = queue->rear();
    QueueWidget* w = qobject_cast<QueueWidget*>(ui->queueDisplay);
    if (w) {
        w->highlightNode(val);
    }
    setQueueStatus(QString("🔍 Back: %1").arg(val));
    QTimer::singleShot(2000, this, [w]() {
        if (w) w->clearHighlight();
    });
}

void MainWindow::on_queueBtnClear_clicked() {
    queue->clear();
    refreshQueue();
    setQueueStatus("Queue limpiada.");
}

// SLOTS: DOUBLE LINKED LIST
void MainWindow::on_dllBtnInsertHead_clicked() {
    bool ok;
    int val = ui->dllInputValue->text().toInt(&ok);
    if (!ok) {
        setDLLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    dll->insertAtHead(val);
    refreshDLL();
    setDLLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 insertado al inicio.").arg(val));
    ui->dllInputValue->clear();
}

void MainWindow::on_dllBtnInsertTail_clicked() {
    bool ok;
    int val = ui->dllInputValue->text().toInt(&ok);
    if (!ok) {
        setDLLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    dll->insertAtTail(val);
    refreshDLL();
    setDLLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 insertado al final.").arg(val));
    ui->dllInputValue->clear();
}

void MainWindow::on_dllBtnInsertAt_clicked() {
    bool okVal, okIdx;
    int val = ui->dllInputValue->text().toInt(&okVal);
    int idx = ui->dllInputIndex->text().toInt(&okIdx);
    if (!okVal || !okIdx) {
        setDLLStatus("( ˶°ㅁ°) !! Ingresa valor e índice válidos.", false);
        return;
    }
    dll->insertAt(idx, val);
    refreshDLL();
    setDLLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 insertado en posición %2.").arg(val).arg(idx));
    ui->dllInputValue->clear();
    ui->dllInputIndex->clear();
}

void MainWindow::on_dllBtnRemove_clicked() {
    bool ok;
    int val = ui->dllInputValue->text().toInt(&ok);
    if (!ok) {
        setDLLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    if (dll->remove(val)) {
        refreshDLL();
        setDLLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ Nodo %1 eliminado.").arg(val));
    } else {
        setDLLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
    }
    ui->dllInputValue->clear();
}

void MainWindow::on_dllBtnSearch_clicked() {
    bool ok;
    int val = ui->dllInputValue->text().toInt(&ok);
    if (!ok) {
        setDLLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    DLLWidget* w = qobject_cast<DLLWidget*>(ui->dllDisplay);
    if (dll->search(val)) {
        if (w) w->highlightNode(val);
        setDLLStatus(QString("🔍 Valor %1 encontrado.").arg(val));
        QTimer::singleShot(2000, this, [w]() {
            if (w) w->clearHighlight();
        });
    } else {
        setDLLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
    }
}

void MainWindow::on_dllBtnClear_clicked() {
    dll->clear();
    refreshDLL();
    setDLLStatus("Lista limpiada.");
}

// SLOTS: BST
void MainWindow::on_bstBtnInsert_clicked() {
    bool ok;
    int val = ui->bstInputValue->text().toInt(&ok);
    if (!ok) {
        setBSTStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    bst->insert(val);
    refreshBST();
    setBSTStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 insertado.").arg(val));
    ui->bstInputValue->clear();
    BSTWidget* w = qobject_cast<BSTWidget*>(ui->bstDisplay);
    if (w) {
        w->animateInsertion(val);
    }
}

void MainWindow::on_bstBtnRemove_clicked() {
    bool ok;
    int val = ui->bstInputValue->text().toInt(&ok);
    if (!ok) {
        setBSTStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    if (!bst->search(val)) {
        setBSTStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
        return;
    }
    setBSTStatus(QString("Eliminando %1...").arg(val));
    ui->bstInputValue->clear();
    BSTWidget* w = qobject_cast<BSTWidget*>(ui->bstDisplay);
    if (w) {
        w->animateDeletion(val);
    }
}

void MainWindow::onBSTDeletionDone(int val) {
    bst->remove(val);
    refreshBST();
    setBSTStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ Nodo %1 eliminado.").arg(val));
}

void MainWindow::on_bstBtnSearch_clicked() {
    bool ok;
    int val = ui->bstInputValue->text().toInt(&ok);
    if (!ok) {
        setBSTStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    BSTWidget* w = qobject_cast<BSTWidget*>(ui->bstDisplay);
    if (bst->search(val)) {
        if (w) {
            w->highlightNode(val);
        }
        setBSTStatus(QString("🔍 Valor %1 encontrado.").arg(val));
        QTimer::singleShot(2000, this, [w]() {
            if (w) w->clearHighlight();
        });
    } else {
        setBSTStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
    }
}

void MainWindow::on_bstBtnClear_clicked() {
    bst->clear();
    refreshBST();
    ui->bstTraversalResult->setText("");
    setBSTStatus("Árbol limpiado.");
}

void MainWindow::on_bstBtnInorder_clicked() {
    if (bst->isEmpty()) {
        setBSTStatus("( ˶°ㅁ°) !! El árbol está vacío.", false);
        return;
    }
    int arr[128]; int idx = 0;
    bst->inorder(bst->getRoot(), arr, idx);
    ui->bstTraversalResult->setText("Inorder: " + traversalToString(arr, idx));
    setBSTStatus("Recorrido inorder completado.");
}

void MainWindow::on_bstBtnPreorder_clicked() {
    if (bst->isEmpty()) {
        setBSTStatus("( ˶°ㅁ°) !! El árbol está vacío.", false);
        return;
    }
    int arr[128]; int idx = 0;
    bst->preorder(bst->getRoot(), arr, idx);
    ui->bstTraversalResult->setText("Preorder: " + traversalToString(arr, idx));
    setBSTStatus("Recorrido preorder completado.");
}

void MainWindow::on_bstBtnPostorder_clicked() {
    if (bst->isEmpty()) {
        setBSTStatus("( ˶°ㅁ°) !! El árbol está vacío.", false);
        return;
    }
    int arr[128]; int idx = 0;
    bst->postorder(bst->getRoot(), arr, idx);
    ui->bstTraversalResult->setText("Postorder: " + traversalToString(arr, idx));
    setBSTStatus("Recorrido postorder completado.");
}

void MainWindow::on_bstBtnSuccessor_clicked() {
    bool ok;
    int val = ui->bstInputValue->text().toInt(&ok);
    if (!ok) {
        setBSTStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }

    BSTNode* curr = bst->getRoot();
    while (curr) {
        if (curr->value == val) {
            break;
        } else if (val < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    if (!curr) {
        setBSTStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
        return;
    }
    if (!curr->right) {
        setBSTStatus(QString("(◞‸ ◟)💧 %1 no tiene sucesor.").arg(val), false);
        return;
    }

    BSTNode* succ = bst->successor(curr);
    BSTWidget* w = qobject_cast<BSTWidget*>(ui->bstDisplay);
    if (w) {
        w->highlightNode(succ->value);
    }
    setBSTStatus(QString("🔍 Sucesor de %1: %2").arg(val).arg(succ->value));
    QTimer::singleShot(2000, this, [w]() {
        if (w) w->clearHighlight();
    });
}

void MainWindow::on_bstBtnPredecessor_clicked() {
    bool ok;
    int val = ui->bstInputValue->text().toInt(&ok);
    if (!ok) {
        setBSTStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }

    BSTNode* curr = bst->getRoot();
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

    if (!curr) {
        setBSTStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
        return;
    }
    if (!curr->left) {
        setBSTStatus(QString("(◞‸ ◟)💧 %1 no tiene predecesor.").arg(val), false);
        return;
    }

    BSTNode* pred = bst->predecessor(curr);
    BSTWidget* w = qobject_cast<BSTWidget*>(ui->bstDisplay);
    if (w) {
        w->highlightNode(pred->value);
    }
    setBSTStatus(QString("🔍 Predecesor de %1: %2").arg(val).arg(pred->value));
    QTimer::singleShot(2000, this, [w]() {
        if (w) w->clearHighlight();
    });
}

void MainWindow::on_bstBtnConvert_clicked() {
    if (bst->isEmpty()) {
        setBSTStatus("( ˶°ㅁ°) !! El árbol está vacío.", false);
        return;
    }

    // 1) Recoger valores del BST en inorder
    conversionIdx = 0;
    conversionTotal = 0;
    bst->inorder(bst->getRoot(), conversionVals, conversionTotal);

    // 2) Limpiar el AVL y cambiar al tab
    avl->clear();
    refreshAVL();
    ui->tabWidget->setCurrentWidget(ui->tabAVL);
    setBSTStatus("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ Convirtiendo a AVL...");
    setAVLStatus("Recibiendo nodos del BST...");

    // 3) Insertar el primer nodo
    conversionStep();
}

// Inserta el siguiente nodo de la conversión, detecta si hubo rotación
// y arranca su animación
void MainWindow::conversionStep() {
    if (conversionIdx >= conversionTotal) {
        setAVLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ BST convertido a AVL (%1 nodos).").arg(conversionTotal));
        return;
    }
    int val = conversionVals[conversionIdx];
    conversionIdx++;

    // Profundidad esperada SI NO hubiera rotación: la cantidad de pasos
    // que tomaría una búsqueda normal desde la raíz actual hasta llegar
    // a una posición vacía (igual que en un BST sin rebalanceo).
    int expectedDepth = 0;
    AVLNode* curr = avl->getRoot();
    while (curr) {
        expectedDepth++;
        if (val < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    conversionExpectedDepth = expectedDepth;

    avl->insert(val);
    refreshAVL();

    // Si la profundidad real del nodo insertado quedó por debajo de la
    // esperada, el rebalanceo lo "subió" en el árbol: hubo una rotación.
    int actualDepth = avl->depth(avl->getRoot(), val);
    if (actualDepth < conversionExpectedDepth) {
        setAVLStatus(QString("🔄 Rotación aplicada al insertar %1 (de profundidad %2 a %3).")
                         .arg(val).arg(conversionExpectedDepth).arg(actualDepth));
    } else {
        setAVLStatus(QString("Insertando %1 desde el BST...").arg(val));
    }

    AVLWidget* w = qobject_cast<AVLWidget*>(ui->avlDisplay);
    if (w) {
        w->animateInsertion(val);
    }
}

// Se llama cuando el AVLWidget termina la animación de un nodo
void MainWindow::onAVLInsertionDone() {
    refreshAVL();
    if (conversionIdx < conversionTotal) {
        conversionStep();
    }
}

// SLOTS: AVL
void MainWindow::on_avlBtnInsert_clicked() {
    bool ok;
    int val = ui->avlInputValue->text().toInt(&ok);
    if (!ok) {
        setAVLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    avl->insert(val);
    refreshAVL();
    setAVLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ %1 insertado.").arg(val));
    ui->avlInputValue->clear();
    AVLWidget* w = qobject_cast<AVLWidget*>(ui->avlDisplay);
    if (w) {
        w->animateInsertion(val);
    }
}

void MainWindow::on_avlBtnRemove_clicked() {
    bool ok;
    int val = ui->avlInputValue->text().toInt(&ok);
    if (!ok) {
        setAVLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    if (!avl->search(val)) {
        setAVLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
        return;
    }
    setAVLStatus(QString("Eliminando %1...").arg(val));
    ui->avlInputValue->clear();
    AVLWidget* w = qobject_cast<AVLWidget*>(ui->avlDisplay);
    if (w) {
        w->animateDeletion(val);
    }
}

// Slot que recibe la señal del widget cuando termina la animación de eliminación
void MainWindow::onAVLDeletionDone(int val) {
    avl->remove(val);
    refreshAVL();
    setAVLStatus(QString("ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ Nodo %1 eliminado.").arg(val));
}

void MainWindow::on_avlBtnSearch_clicked() {
    bool ok;
    int val = ui->avlInputValue->text().toInt(&ok);
    if (!ok) {
        setAVLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }
    AVLWidget* w = qobject_cast<AVLWidget*>(ui->avlDisplay);
    if (avl->search(val)) {
        if (w) {
            w->highlightNode(val);
        }
        setAVLStatus(QString("🔍 Valor %1 encontrado.").arg(val));
        QTimer::singleShot(2000, this, [w]() {
            if (w) w->clearHighlight();
        });
    } else {
        setAVLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
    }
}

void MainWindow::on_avlBtnClear_clicked() {
    avl->clear();
    refreshAVL();
    ui->avlTraversalResult->setText("");
    setAVLStatus("Árbol limpiado.");
}

void MainWindow::on_avlBtnInorder_clicked() {
    if (avl->isEmpty()) {
        setAVLStatus("( ˶°ㅁ°) !! El árbol está vacío.", false);
        return;
    }
    int arr[128]; int idx = 0;
    avl->inorder(avl->getRoot(), arr, idx);
    ui->avlTraversalResult->setText("Inorder: " + traversalToString(arr, idx));
    setAVLStatus("Recorrido inorder completado.");
}

void MainWindow::on_avlBtnPreorder_clicked() {
    if (avl->isEmpty()) {
        setAVLStatus("( ˶°ㅁ°) !! El árbol está vacío.", false);
        return;
    }
    int arr[128]; int idx = 0;
    avl->preorder(avl->getRoot(), arr, idx);
    ui->avlTraversalResult->setText("Preorder: " + traversalToString(arr, idx));
    setAVLStatus("Recorrido preorder completado.");
}

void MainWindow::on_avlBtnPostorder_clicked() {
    if (avl->isEmpty()) {
        setAVLStatus("( ˶°ㅁ°) !! El árbol está vacío.", false);
        return;
    }
    int arr[128]; int idx = 0;
    avl->postorder(avl->getRoot(), arr, idx);
    ui->avlTraversalResult->setText("Postorder: " + traversalToString(arr, idx));
    setAVLStatus("Recorrido postorder completado.");
}

void MainWindow::on_avlBtnSuccessor_clicked() {
    bool ok;
    int val = ui->avlInputValue->text().toInt(&ok);
    if (!ok) {
        setAVLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }

    AVLNode* curr = avl->getRoot();
    while (curr) {
        if (curr->value == val) {
            break;
        } else if (val < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    if (!curr) {
        setAVLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
        return;
    }
    if (!curr->right) {
        setAVLStatus(QString("(◞‸ ◟)💧 %1 no tiene sucesor.").arg(val), false);
        return;
    }

    AVLNode* succ = avl->successor(curr);
    AVLWidget* w = qobject_cast<AVLWidget*>(ui->avlDisplay);
    if (w) {
        w->highlightNode(succ->value);
    }
    setAVLStatus(QString("🔍 Sucesor de %1: %2").arg(val).arg(succ->value));
    QTimer::singleShot(2000, this, [w]() {
        if (w) w->clearHighlight();
    });
}

void MainWindow::on_avlBtnPredecessor_clicked() {
    bool ok;
    int val = ui->avlInputValue->text().toInt(&ok);
    if (!ok) {
        setAVLStatus("( ˶°ㅁ°) !! Ingresa un número entero válido.", false);
        return;
    }

    AVLNode* curr = avl->getRoot();
    while (curr) {
        if (curr->value == val) {
            break;
        } else if (val < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    if (!curr) {
        setAVLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
        return;
    }
    if (!curr->left) {
        setAVLStatus(QString("(◞‸ ◟)💧 %1 no tiene predecesor.").arg(val), false);
        return;
    }

    AVLNode* pred = avl->predecessor(curr);
    AVLWidget* w = qobject_cast<AVLWidget*>(ui->avlDisplay);
    if (w) {
        w->highlightNode(pred->value);
    }
    setAVLStatus(QString("🔍 Predecesor de %1: %2").arg(val).arg(pred->value));
    QTimer::singleShot(2000, this, [w]() {
        if (w) w->clearHighlight();
    });
}

// SLOTS: PERSISTENCIA
void MainWindow::on_btnSave_clicked() {
    QString path = QFileDialog::getSaveFileName(
        this, "Guardar estructuras", "", "Archivos binarios (*.bin)");
    if (path.isEmpty()) {
        return;
    }

    bool ok = Persistence::save(path.toStdString(), list, stack, queue, dll, bst, avl);
    if (ok) {
        QMessageBox::information(this, "StructViz", "ദ്ദി(ᵔᗜᵔ) Estructuras guardadas correctamente.");
    } else {
        QMessageBox::warning(this, "StructViz", "( •̯́ ₃ •̯̀) No se pudo guardar el archivo.");
    }
}

void MainWindow::on_btnLoad_clicked() {
    QString path = QFileDialog::getOpenFileName(
        this, "Cargar estructuras", "", "Archivos binarios (*.bin)");
    if (path.isEmpty()) {
        return;
    }

    bool ok = Persistence::load(path.toStdString(), list, stack, queue, dll, bst, avl);
    if (ok) {
        refreshLL();
        refreshStack();
        refreshQueue();
        refreshDLL();
        refreshBST();
        refreshAVL();
        QMessageBox::information(this, "StructViz", "ദ്ദി(ᵔᗜᵔ) Estructuras cargadas correctamente.");
    } else {
        QMessageBox::warning(this, "StructViz", "( •̯́ ₃ •̯̀) No se pudo cargar el archivo.");
    }
}
