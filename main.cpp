#include <QApplication>
#include "ui/MainWindow.h"

// PUNTO DE ENTRADA
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
