#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "ps_lsquicserver.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLineEdit* m_PORTLE;

    QPushButton* m_listenBtn;
    QPushButton* m_clearBtn;

    QPlainTextEdit* m_status;

    QuicServerShared m_server;

    QFile m_log;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void textSlot(QString);
    void logText(QString);
};
#endif // MAINWINDOW_H
