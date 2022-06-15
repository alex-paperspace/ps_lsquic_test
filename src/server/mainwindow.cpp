#include "mainwindow.h"
#include "logger.h"

#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_log("serverdebug")
{
    setMinimumSize(500,800);

    QWidget* centralwgt = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(centralwgt);
    QHBoxLayout* inputlay = new QHBoxLayout;

    m_PORTLE = new QLineEdit;
    m_PORTLE->setPlaceholderText("PORT");
    inputlay->addWidget(m_PORTLE);
    lay->addLayout(inputlay);

    m_status = new QPlainTextEdit;
    m_status->setReadOnly(true);

    paperspace::lsquic::Logger* loggerptr = &paperspace::lsquic::Logger::getInstance();
    connect(loggerptr, &paperspace::lsquic::Logger::textSignal, this, &MainWindow::textSlot);

    if (!m_log.open(QIODevice::WriteOnly | QIODevice::Text)) {
        loggerptr->LOG("Could not open log device");
    }
    connect(loggerptr, &paperspace::lsquic::Logger::textSignal, this, &MainWindow::logText);

    m_server = QuicServerShared(new QuicServer);

    m_listenBtn = new QPushButton("Listen");
    connect(m_listenBtn, &QPushButton::clicked, this, [=]{
        m_status->clear();
        m_server->setListenPort(m_PORTLE->text());
        m_server->listen();

    });
    lay->addWidget(m_listenBtn);

    lay->addWidget(m_status);

    m_clearBtn = new QPushButton("Clear");
    connect(m_clearBtn, &QPushButton::clicked, this, [=]{
        m_status->clear();
    });
    lay->addWidget(m_clearBtn);

    setCentralWidget(centralwgt);
}

MainWindow::~MainWindow()
{
    m_log.close();
}

void MainWindow::textSlot(QString str)
{
    m_status->appendPlainText(str);
}

void MainWindow::logText(QString txt)
{
    if (m_log.isWritable()) {
        QTextStream ts(&m_log);
        ts << txt << '\n';
        ts.flush();
        m_log.flush();
    }
}

