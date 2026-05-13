#pragma once
#include <QDialog>
#include "../logic/LinkedList.h"
#include "LinkedListWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnInsertHead_clicked();
    void on_btnInsertTail_clicked();
    void on_btnInsertAt_clicked();
    void on_btnRemove_clicked();
    void on_btnSearch_clicked();
    void on_btnClear_clicked();

private:
    Ui::MainWindow* ui;
    LinkedList* list;

    void refreshList();
    void setStatus(const QString& msg, bool ok = true);
};
