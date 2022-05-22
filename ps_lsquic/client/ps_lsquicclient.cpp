#include "ps_lsquicclient.h"
#include "common/logger.h"

#include <unistd.h>

namespace paperspace {
namespace lsquic {

PS_LSQuicClient::PS_LSQuicClient() :
    m_conn(nullptr)
{
    memset(&m_eapi, 0, sizeof(m_eapi));

    m_eapi.ea_packets_out = util::packets_out;
    m_eapi.ea_packets_out_ctx = this;

    m_eapi.ea_stream_if = m_cbs.getInterface();
    m_eapi.ea_stream_if_ctx = nullptr;

    //PS_LSQuicSSL::getInstance().load_cert("","");

    m_eapi.ea_get_ssl_ctx = get_ssl_ctx;
    m_eapi.ea_settings = NULL; //use defaults, can change later

    m_engine = QuicEngineShared(new PS_LSQuicEngine(m_eapi, false));


}

PS_LSQuicClient::~PS_LSQuicClient()
{
    cleanup();
    if ((*m_engine).isValid() && (*m_engine).engine()) {
        lsquic_engine_destroy((*m_engine).engine());
    }

}

void PS_LSQuicClient::connect()
{
    cleanup();

    if (m_targetIPStr.isEmpty() || m_targetPortStr.isEmpty()) {
        Logger::getInstance().LOG("Empty input. Aborting...");
        return;
    }

    if (!m_engine->isValid()) {
        Logger::getInstance().LOG("Engine not valid. Aborting...");
        cleanup();
        return;
    }

    //parse addr
    memset(&m_targetAddr.addr4, 0, sizeof(m_targetAddr.addr4));
    QByteArray ipBA = m_targetIPStr.toLocal8Bit();
    int port = m_targetPortStr.toInt();
    if (inet_pton(AF_INET, ipBA.data(), &m_targetAddr.addr4.sin_addr))
    {
        Logger::getInstance().LOG("IPV4 address detected.");
        m_targetAddr.addr4.sin_family = AF_INET;
        m_targetAddr.addr4.sin_port = htons(port);
    } else if (memset(&m_targetAddr.addr6, 0, sizeof(m_targetAddr.addr6)),
               inet_pton(AF_INET6, ipBA.data(), &m_targetAddr.addr6.sin6_addr))
    {
        Logger::getInstance().LOG("IPV6 address detected.");
        m_targetAddr.addr6.sin6_family = AF_INET6;
        m_targetAddr.addr6.sin6_port = htons(port);
    } else {
        Logger::getInstance().LOG("Not a valid IP Address. Aborting...");
        cleanup();
        return;
    }

    Logger::getInstance().LOG("Trying to connect to " + m_targetIPStr + ":" + m_targetPortStr + "...");

    //socket
    m_fd = socket(m_targetAddr.sa.sa_family, SOCK_DGRAM, 0);

    if (m_fd < 0) {
        Logger::getInstance().LOG("Failed to create socket. Aborting...");
        cleanup();
        return;
    }

    int socklen = sizeof(m_local_sa);
    m_local_sa.sin_addr.s_addr = INADDR_ANY;
    m_local_sa.sin_family = AF_INET;
    if (0 != bind(m_fd, (sockaddr*)&m_local_sa, socklen)) {
        Logger::getInstance().LOGF("Failed to bind socket on %d. errno: %d Aborting...", m_fd, errno);
        cleanup();
        return;
    }
    if (m_sock) {
        Logger::getInstance().LOG("Socket not clean. Aborting...");
        cleanup();
        return;
    }

    m_sock = QSharedPointer<QAbstractSocket>(new QAbstractSocket(QAbstractSocket::UdpSocket, nullptr));
    m_sock->setSocketDescriptor(m_fd, QAbstractSocket::UnconnectedState);
//    if (!(*m_sock).bind(QHostAddress::LocalHost)) {
//        Logger::getInstance().LOG("Failed to bind socket. Aborting...");
//        cleanup();
//        return;
//    }

    QObject::connect(m_sock.data(), &QUdpSocket::readyRead, [=]{
        Logger::getInstance().LOG("Stuff in socket");
    });

    m_conn = lsquic_engine_connect((*m_engine).engine(),            //engine
                                   LSQVER_043,                      //version
                                   (sockaddr*) &m_local_sa,        //local sockaddr
                                   &m_targetAddr.sa,                //peer sockaddr
                                   (void*) getSockFD(),             //peer ctx
                                   nullptr,                         //conn ctx
                                   "psrmw8ycz.vm.paperspace.com",   //sni
                                   0,                               //plumptu
                                   nullptr,                         //session resume
                                   0,                               //session resume len
                                   nullptr,                         //token
                                   0);                              //token len

    if (!m_conn) {
        Logger::getInstance().LOG("Could not create connection. Aborting...");
        cleanup();
        return;
    }
    process_conns();

}

int PS_LSQuicClient::writeDatagram(const QNetworkDatagram &dg)
{
    //return m_sock->writeDatagram(dg);
}

int PS_LSQuicClient::getSockFD()
{
    //return m_fd;
    if (m_sock.isNull()) {
        return -1;
    }

    return (*m_sock).socketDescriptor();
}

const QString &PS_LSQuicClient::targetIPStr() const
{
    return m_targetIPStr;
}

const QString &PS_LSQuicClient::targetPortStr() const
{
    return m_targetPortStr;
}

void PS_LSQuicClient::cleanup()
{
    Logger::getInstance().LOG("Cleaning up.");
//    if (m_fd) {
//        close(m_fd);
//        m_fd = 0;
//    }
    if (!m_sock.isNull()) {
        m_sock.clear();
    }
}

}
}


