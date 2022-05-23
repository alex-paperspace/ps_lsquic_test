#include "mainwindow.h"

#include <QApplication>
#include <QtGlobal>

#include "common/ps_lsquic_lib_initializer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!paperspace::lsquic::initialize_lsquic()) {
        a.exit(1);
    }


#ifdef Q_OS_WIN
    WORD wVersionRequested;
    wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);
#endif

    MainWindow w;
    w.show();
    return a.exec();
}
