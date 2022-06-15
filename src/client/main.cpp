#include "mainwindow.h"

#include <QApplication>

#include "ps_lsquic_lib_initializer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!paperspace::lsquic::initialize_lsquic()) {
        a.exit(1);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
