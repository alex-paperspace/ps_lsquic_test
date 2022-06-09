#include "mainwindow.h"
#include "common/logger.h"

#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_log("clientdebug")
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
    m_SNILE = new QLineEdit;
    m_SNILE->setPlaceholderText("SNI");
    inputlay->addWidget(m_IPLE);
    inputlay->addWidget(m_PORTLE);
    lay->addLayout(inputlay);
    lay->addWidget(m_SNILE);

    QHBoxLayout* chatlay = new QHBoxLayout;
    m_chatIn = new QPlainTextEdit;
    m_chatIn->setReadOnly(false);
    m_chatIn->setPlaceholderText("Input Chat Area.");
    chatlay->addWidget(m_chatIn);

    m_chatOut = new QPlainTextEdit;
    m_chatOut->setReadOnly(true);
    m_chatOut->setPlaceholderText("Output Chat Area.");
    chatlay->addWidget(m_chatOut);

    m_status = new QPlainTextEdit;
    m_status->setReadOnly(true);

    paperspace::lsquic::Logger* loggerptr = &paperspace::lsquic::Logger::getInstance();
    connect(loggerptr, &paperspace::lsquic::Logger::textSignal, this, &MainWindow::statusTextSlot);

    if (!m_log.open(QIODevice::WriteOnly | QIODevice::Text)) {
        loggerptr->LOG("Could not open log device");
    }
    connect(loggerptr, &paperspace::lsquic::Logger::textSignal, [=](QString txt){
        QTextStream ts(&m_log);
        ts << txt << '\n';
        ts.flush();
        m_log.flush();
    });

    m_client = QuicClientShared(new QuicClient);
    connect(&*m_client, &QuicClient::receivedDataSignal, this, [=](QByteArray data){
        QString dataStr = QString::fromUtf8(data);
        m_chatOut->appendPlainText(dataStr);
    });

    m_connectBtn = new QPushButton("Connect");
    connect(m_connectBtn, &QPushButton::clicked, this, [=]{
        m_status->clear();
        m_client->setIP(m_IPLE->text());
        m_client->setPort(m_PORTLE->text());
        m_client->setSNI(m_SNILE->text());
        m_client->connect();
    });
    m_disconnectBtn = new QPushButton("Disconnect");
    connect(m_disconnectBtn, &QPushButton::clicked, this, [=]{
        m_client->disconnect();
    });
    m_sendBtn = new QPushButton("Send");
    connect(m_sendBtn, &QPushButton::clicked, this, [=]{
        QByteArray data = m_chatIn->toPlainText().toUtf8();
        m_chatIn->clear();
        m_client->sendData(data);
    });
    controllay->addWidget(m_connectBtn);
    controllay->addWidget(m_disconnectBtn);
    lay->addLayout(controllay);

    lay->addLayout(chatlay);
    lay->addWidget(m_sendBtn);
    lay->addWidget(m_status);

    setCentralWidget(centralwgt);
}

MainWindow::~MainWindow()
{
}

void MainWindow::statusTextSlot(QString str)
{
    m_status->appendPlainText(str);
}
