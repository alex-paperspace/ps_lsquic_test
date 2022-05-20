#include "mainwindow.h"

#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_client(QuicClientShared(new QuicClient))
{

    QWidget* centralwgt = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(centralwgt);
    QHBoxLayout* inputlay = new QHBoxLayout;

    m_IPLE = new QLineEdit;
    m_IPLE->setPlaceholderText("IP");
    m_PORTLE = new QLineEdit;
    m_PORTLE->setPlaceholderText("PORT");
    inputlay->addWidget(m_IPLE);
    inputlay->addWidget(m_PORTLE);
    lay->addLayout(inputlay);

    m_connectBtn = new QPushButton("Connect");
    connect(m_connectBtn, &QPushButton::clicked, this, [=]{
        m_client->setIP(m_IPLE->text());
        m_client->setPort(m_PORTLE->text());
        //m_client->connect();
    });
    lay->addWidget(m_connectBtn);

    m_status = new QTextEdit;
    m_status->setReadOnly(true);
    lay->addWidget(m_status);

    setCentralWidget(centralwgt);
}

MainWindow::~MainWindow()
{
}

