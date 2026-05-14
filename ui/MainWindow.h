#pragma once
#include <QDialog>
#include "../logic/LinkedList.h"
#include "LinkedListWidget.h"

// VENTANA PRINCIPAL
// Conecta la lógica con la UI
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // -Slots conectados a cada botón del .ui-
    void on_btnInsertHead_clicked();
    void on_btnInsertTail_clicked();
    void on_btnInsertAt_clicked();
    void on_btnRemove_clicked();
    void on_btnSearch_clicked();
    void on_btnClear_clicked();

private:
    Ui::MainWindow* ui;
    LinkedList* list;

    // -Helpers-
    void refreshList();
    void setStatus(const QString& msg, bool ok = true);
};
