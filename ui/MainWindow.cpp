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
{
    ui->setupUi(this);
    setWindowTitle("StructViz");

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

    // Conectar widgets con sus estructuras
    refreshLL();
    refreshStack();
    refreshQueue();
    refreshDLL();
}

// DESTRUCTOR
MainWindow::~MainWindow() {
    delete ui;
    delete list;
    delete stack;
    delete queue;
    delete dll;
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
        QTimer::singleShot(2000, this, [w]() { if (w) w->clearHighlight(); });
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
    QTimer::singleShot(2000, this, [w]() { if (w) w->clearHighlight(); });
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
    QTimer::singleShot(2000, this, [w]() { if (w) w->clearHighlight(); });
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
    QTimer::singleShot(2000, this, [w]() { if (w) w->clearHighlight(); });
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
        if (w) {
            w->highlightNode(val);
        }
        setDLLStatus(QString("🔍 Valor %1 encontrado.").arg(val));
        QTimer::singleShot(2000, this, [w]() { if (w) w->clearHighlight(); });
    } else {
        setDLLStatus(QString("(◞‸ ◟)💧 Valor %1 no encontrado.").arg(val), false);
    }
}

void MainWindow::on_dllBtnClear_clicked() {
    dll->clear();
    refreshDLL();
    setDLLStatus("Lista limpiada.");
}

// SLOTS: PERSISTENCIA

void MainWindow::on_btnSave_clicked() {
    QString path = QFileDialog::getSaveFileName(
        this, "Guardar estructuras", "", "Archivos de texto (*.txt)");
    if (path.isEmpty()) {
        return;
    }

    bool ok = Persistence::save(path.toStdString(), list, stack, queue, dll);
    if (ok) {
        QMessageBox::information(this, "StructViz", "ദ്ദി(ᵔᗜᵔ) Estructuras guardadas correctamente.");
    } else {
        QMessageBox::warning(this, "StructViz", "( •̯́ ₃ •̯̀) No se pudo guardar el archivo.");
    }
}

void MainWindow::on_btnLoad_clicked() {
    QString path = QFileDialog::getOpenFileName(
        this, "Cargar estructuras", "", "Archivos de texto (*.txt)");
    if (path.isEmpty()) {
        return;
    }

    bool ok = Persistence::load(path.toStdString(), list, stack, queue, dll);
    if (ok) {
        refreshLL();
        refreshStack();
        refreshQueue();
        refreshDLL();
        QMessageBox::information(this, "StructViz", "ദ്ദി(ᵔᗜᵔ) Estructuras cargadas correctamente.");
    } else {
        QMessageBox::warning(this, "StructViz", "( •̯́ ₃ •̯̀) No se pudo cargar el archivo.");
    }
}
