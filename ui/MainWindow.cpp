#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTimer>

// CONSTRUCTOR
// Inicializa la UI generada por Qt Designer
MainWindow::MainWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
    , list(new LinkedList())
{
    ui->setupUi(this);
    setWindowTitle("StructViz");

    // Stylesheet
    setStyleSheet(R"(
        QDialog {
            background-color: #ede6f5;
        }
        QLabel#lblTitulo {
            color: #7a5fa0;
            font-size: 18px;
            font-weight: bold;
            font-family: 'Segoe UI';
        }
        QLabel#lblSize {
            color: #8a70b0;
            font-size: 12px;
            font-family: 'Segoe UI';
        }
        QLabel#lblStatus {
            color: #6a4f90;
            font-size: 12px;
            font-family: 'Segoe UI';
            background-color: #e8d8f8;
            border-radius: 8px;
            padding: 6px 12px;
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
        QPushButton#btnInsertHead {
            background-color: #e8d8f8;
            border: 1.5px solid #c0a8e0;
            border-radius: 10px;
            color: #5a3f8a;
            font-family: 'Segoe UI';
            font-weight: bold;
            padding: 6px 12px;
        }
        QPushButton#btnInsertTail {
            background-color: #f0d8eb;
            border: 1.5px solid #d0a8c8;
            border-radius: 10px;
            color: #8a3f70;
            font-family: 'Segoe UI';
            font-weight: bold;
            padding: 6px 12px;
        }
        QPushButton#btnInsertAt {
            background-color: #ddd0f8;
            border: 1.5px solid #b09edc;
            border-radius: 10px;
            color: #5a3f8a;
            font-family: 'Segoe UI';
            font-weight: bold;
            padding: 6px 12px;
        }
        QPushButton#btnRemove {
            background-color: #fadadd;
            border: 1.5px solid #e8b0b8;
            border-radius: 10px;
            color: #8a3f4f;
            font-family: 'Segoe UI';
            font-weight: bold;
            padding: 6px 12px;
        }
        QPushButton#btnSearch {
            background-color: #d8eef0;
            border: 1.5px solid #a8ccd8;
            border-radius: 10px;
            color: #3f6f8a;
            font-family: 'Segoe UI';
            font-weight: bold;
            padding: 6px 12px;
        }
        QPushButton#btnClear {
            background-color: #fdebd0;
            border: 1.5px solid #e8c898;
            border-radius: 10px;
            color: #8a6030;
            font-family: 'Segoe UI';
            font-weight: bold;
            padding: 6px 12px;
        }
        QPushButton:pressed {
            padding: 7px 11px 5px 13px;
        }
    )");

    refreshList();
}

// DESTRUCTOR
// Libera la UI generada y la lista lógica
MainWindow::~MainWindow() {
    delete ui;
    delete list;
}

// REFRESHLIST
// Sincroniza el widget visual con el estado actual de la lista
void MainWindow::refreshList() {
    LinkedListWidget* w = qobject_cast<LinkedListWidget*>(ui->displayBox);
    if (w) w->setList(list);

    if (!list->getHead()) {
        ui->lblSize->setText("Tamaño: 0 nodos");
        return;
    }
    ui->lblSize->setText(QString("Tamaño: %1 nodo%2")
                             .arg(list->size())
                             .arg(list->size() == 1 ? "" : "s"));
}

// SETSTATUS
// Muestra un mensaje en lblStatus
// verde = exitosa, rojo = NO exitosa
void MainWindow::setStatus(const QString& msg, bool ok) {
    ui->lblStatus->setText(msg);
    ui->lblStatus->setStyleSheet(
        ok ? "color: #5a8a5a; font-weight: bold;"
           : "color: #a03040; font-weight: bold;"
        );
}

// SLOTS DE BOTONES
void MainWindow::on_btnInsertHead_clicked() {
    bool ok;
    int val = ui->inputValue->text().toInt(&ok);
    if (!ok) { setStatus("⚠ Ingresa un número entero válido.", false); return; }

    list->insertAtHead(val);
    refreshList();
    setStatus(QString("✔ %1 insertado al inicio.").arg(val));
    ui->inputValue->clear();
}

void MainWindow::on_btnInsertTail_clicked() {
    bool ok;
    int val = ui->inputValue->text().toInt(&ok);
    if (!ok) { setStatus("⚠ Ingresa un número entero válido.", false); return; }

    list->insertAtTail(val);
    refreshList();
    setStatus(QString("✔ %1 insertado al final.").arg(val));
    ui->inputValue->clear();
}

void MainWindow::on_btnInsertAt_clicked() {
    bool okVal, okIdx;
    int val = ui->inputValue->text().toInt(&okVal);
    int idx = ui->inputIndex->text().toInt(&okIdx);
    if (!okVal || !okIdx) { setStatus("⚠ Ingresa valor e índice válidos.", false); return; }

    list->insertAt(idx, val);
    refreshList();
    setStatus(QString("✔ %1 insertado en posición %2.").arg(val).arg(idx));
    ui->inputValue->clear();
    ui->inputIndex->clear();
}

void MainWindow::on_btnRemove_clicked() {
    bool ok;
    int val = ui->inputValue->text().toInt(&ok);
    if (!ok) { setStatus("⚠ Ingresa un número entero válido.", false); return; }

    if (list->remove(val)) {
        refreshList();
        setStatus(QString("✔ Nodo %1 eliminado.").arg(val));
    } else {
        setStatus(QString("✘ Valor %1 no encontrado.").arg(val), false);
    }
    ui->inputValue->clear();
}

void MainWindow::on_btnSearch_clicked() {
    bool ok;
    int val = ui->inputValue->text().toInt(&ok);
    if (!ok) { setStatus("⚠ Ingresa un número entero válido.", false); return; }

    LinkedListWidget* w = qobject_cast<LinkedListWidget*>(ui->displayBox);
    if (list->search(val)) {
        if (w) w->highlightNode(val);
        setStatus(QString("🔍 Valor %1 ENCONTRADO.").arg(val));
        QTimer::singleShot(2000, this, [w]() { if (w) w->clearHighlight(); });
    } else {
        setStatus(QString("✘ Valor %1 NO encontrado.").arg(val), false);
    }
}

void MainWindow::on_btnClear_clicked() {
    list->clear();
    refreshList();
    setStatus("Lista limpiada.");
}
