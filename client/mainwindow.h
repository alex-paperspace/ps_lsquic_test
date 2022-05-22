#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "client/ps_lsquicclient.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLineEdit* m_IPLE;
    QLineEdit* m_PORTLE;

    QPushButton* m_connectBtn;
    QPushButton* m_disconnectBtn;

    QPlainTextEdit* m_status;

    QuicClientShared m_client;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void textSlot(QString);
};
#endif // MAINWINDOW_H
