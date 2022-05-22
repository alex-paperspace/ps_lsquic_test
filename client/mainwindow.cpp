#include "mainwindow.h"
#include "common/logger.h"

#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(500,800);

    QWidget* centralwgt = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(centralwgt);
    QHBoxLayout* inputlay = new QHBoxLayout;
    QHBoxLayout* controllay = new QHBoxLayout;

    m_IPLE = new QLineEdit;
    m_IPLE->setPlaceholderText("IP");
    m_PORTLE = new QLineEdit;
    m_PORTLE->setPlaceholderText("PORT");
    inputlay->addWidget(m_IPLE);
    inputlay->addWidget(m_PORTLE);
    lay->addLayout(inputlay);

    m_status = new QPlainTextEdit;
    m_status->setReadOnly(true);

    paperspace::lsquic::Logger* loggerptr = &paperspace::lsquic::Logger::getInstance();
    connect(loggerptr, &paperspace::lsquic::Logger::textSignal, this, &MainWindow::textSlot);

    m_client = QuicClientShared(new QuicClient);

    m_connectBtn = new QPushButton("Connect");
    connect(m_connectBtn, &QPushButton::clicked, this, [=]{
        m_status->clear();
        m_client->setIP(m_IPLE->text());
        m_client->setPort(m_PORTLE->text());
        m_client->connect();
    });
    m_disconnectBtn = new QPushButton("Disconnect");
    connect(m_disconnectBtn, &QPushButton::clicked, this, [=]{
        m_client->disconnect();
    });
    controllay->addWidget(m_connectBtn);
    controllay->addWidget(m_disconnectBtn);
    lay->addLayout(controllay);

    lay->addWidget(m_status);

    setCentralWidget(centralwgt);
}

MainWindow::~MainWindow()
{
}

void MainWindow::textSlot(QString str)
{
    m_status->appendPlainText(str);
}

