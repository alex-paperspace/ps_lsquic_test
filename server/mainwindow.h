#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "server/ps_lsquicserver.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLineEdit* m_PORTLE;

    QPushButton* m_listenBtn;

    QPlainTextEdit* m_status;

    QuicServerShared m_server;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void textSlot(QString);
};
#endif // MAINWINDOW_H
