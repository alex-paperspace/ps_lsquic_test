#include "mainwindow.h"

#include <QApplication>

#include "common/ps_lsquic_lib_initializer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!paperspace::lsquic::initialize_lsquic()) {
        a.exit(1);
    }

    WORD wVersionRequested;
    wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);

    MainWindow w;
    w.show();
    return a.exec();
}
