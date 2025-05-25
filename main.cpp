#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow w;
    w.setWindowTitle("Pilditöötluse programm");
    w.show();

    return app.exec();
}
