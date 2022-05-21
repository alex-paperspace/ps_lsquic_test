#include "ps_lsquicclient.h"
#include "common/logger.h"

namespace paperspace {
namespace lsquic {

PS_LSQuicClient::PS_LSQuicClient() :
    m_conn(nullptr)
{
    memset(&m_eapi, 0, sizeof(m_eapi));

    m_eapi.ea_packets_out = util::packets_out;
    m_eapi.ea_packets_out_ctx = nullptr;

    m_eapi.ea_stream_if = m_cbs.getInterface();
    m_eapi.ea_stream_if_ctx = nullptr;

    //PS_LSQuicSSL::getInstance().load_cert("","");

    //m_eapi.ea_get_ssl_ctx = get_ssl_ctx;
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
        m_targetAddr.addr4.sin_family = AF_INET;
        m_targetAddr.addr4.sin_port = htons(port);
    } else if (memset(&m_targetAddr.addr6, 0, sizeof(m_targetAddr.addr6)),
               inet_pton(AF_INET6, ipBA.data(), &m_targetAddr.addr6.sin6_addr))
    {
        m_targetAddr.addr6.sin6_family = AF_INET6;
        m_targetAddr.addr6.sin6_port = htons(port);
    } else {
        Logger::getInstance().LOG("Not a valid IP Address. Aborting...");
        cleanup();
        return;
    }
    m_local_sas.ss_family = m_targetAddr.sa.sa_family;

    Logger::getInstance().LOG("Trying to connect to " + m_targetIPStr + ":" + m_targetPortStr + "...");

    //socket
    if (m_sock) {
        Logger::getInstance().LOG("Socket not clean. Aborting...");
        cleanup();
        return;
    }

    m_sock = QSharedPointer<QUdpSocket>(new QUdpSocket);
    if (!(*m_sock).bind(QHostAddress::LocalHost)) {
        Logger::getInstance().LOG("Failed to bind socket. Aborting...");
        cleanup();
        return;
    }

    QObject::connect(m_sock.data(), &QUdpSocket::readyRead, [=]{
        Logger::getInstance().LOG("Stuff in socket");
    });

    m_conn = lsquic_engine_connect((*m_engine).engine(),            //engine
                                   LSQVER_043,                      //version
                                   (sockaddr*) &m_local_sas,        //local sockaddr
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
}

int PS_LSQuicClient::getSockFD()
{
    if (m_sock.isNull()) {
        return -1;
    }

    return (*m_sock).socketDescriptor();
}

void PS_LSQuicClient::cleanup()
{
    if (!m_sock.isNull()) {
        m_sock.clear();
    }
}

}
}


