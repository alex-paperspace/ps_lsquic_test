#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "ps_lsquicclient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLineEdit* m_IPLE;
    QLineEdit* m_PORTLE;
    QLineEdit* m_SNILE;

    QPushButton* m_connectBtn;
    QPushButton* m_disconnectBtn;
    QPushButton* m_sendBtn;

    QPlainTextEdit* m_chatIn;
    QPlainTextEdit* m_chatOut;
    QPlainTextEdit* m_status;

    QuicClientShared m_client;

    QFile m_log;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void statusTextSlot(QString);
    void logText(QString);
};
#endif // MAINWINDOW_H
