#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include "client/ps_lsquicclient.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLineEdit* m_IPLE;
    QLineEdit* m_PORTLE;

    QPushButton* m_connectBtn;

    QTextEdit* m_status;

    QuicClientShared m_client;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
