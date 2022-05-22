#include "ps_lsquicclient.h"
#include "common/logger.h"
#include "common/ps_lsquic_ssl.h"

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
    disconnect();

    if ((*m_engine).isValid() && (*m_engine).engine()) {
        lsquic_engine_destroy((*m_engine).engine());
    }

}

void PS_LSQuicClient::connect()
{
    disconnect();


    if (m_targetIPStr.isEmpty() || m_targetPortStr.isEmpty()) {
        Logger::getInstance().LOG("Empty input. Aborting...");
        return;
    }

    if (!m_engine->isValid()) {
        Logger::getInstance().LOG("Engine not valid. Aborting...");
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
        return;
    }

    Logger::getInstance().LOG("Trying to connect to " + m_targetIPStr + ":" + m_targetPortStr + "...");

    //socket
    m_fd = socket(m_targetAddr.sa.sa_family, SOCK_DGRAM, 0);

    if (m_fd < 0) {
        Logger::getInstance().LOG("Failed to create socket. Aborting...");
        disconnect();
        return;
    }

    int socklen = sizeof(m_local_sa);
    m_local_sa.sin_addr.s_addr = INADDR_ANY;
    m_local_sa.sin_family = AF_INET;
    if (0 != bind(m_fd, (sockaddr*)&m_local_sa, socklen)) {
        Logger::getInstance().LOGF("Failed to bind socket on %d. errno: %d Aborting...", m_fd, errno);
        disconnect();
        return;
    }
    if (m_sock) {
        Logger::getInstance().LOG("Socket not clean. Aborting...");
        disconnect();
        return;
    }

    m_sock = QSharedPointer<QAbstractSocket>(new QAbstractSocket(QAbstractSocket::UdpSocket, nullptr));
    m_sock->setSocketDescriptor(m_fd, QAbstractSocket::UnconnectedState);

    QObject::connect(m_sock.data(), &QAbstractSocket::readyRead, [=]{
        Logger::getInstance().LOG("Stuff in socket");
    });

    m_conn = lsquic_engine_connect((*m_engine).engine(),            //engine
                                   LSQVER_043,                      //version
                                   (sockaddr*) &m_local_sa,        //local sockaddr
                                   &m_targetAddr.sa,                //peer sockaddr
                                   (void*) getSockFD(),             //peer ctx
                                   nullptr,                         //conn ctx
                                   "",   //sni
                                   0,                               //plumptu
                                   nullptr,                         //session resume
                                   0,                               //session resume len
                                   nullptr,                         //token
                                   0);                              //token len

    if (!m_conn) {
        Logger::getInstance().LOG("Could not create connection. Aborting...");
        disconnect();
        return;
    }
    process_conns();

}

void PS_LSQuicClient::disconnect()
{
    Logger::getInstance().LOG("Disconnecting.");
    if (m_conn) {
        lsquic_conn_going_away(m_conn);
        lsquic_conn_close(m_conn);
        m_conn = nullptr;
    }
    cleanup();
}


}
}


