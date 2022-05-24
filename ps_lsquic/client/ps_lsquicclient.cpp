#include "ps_lsquicclient.h"
#include "common/logger.h"
#include "common/ps_lsquic_ssl.h"

#include <QUdpSocket>
#include <QNetworkDatagram>

namespace paperspace {
namespace lsquic {

PS_LSQuicClient::PS_LSQuicClient()
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

    if (m_targetIP.isEmpty() || m_targetPort == -1) {
        Logger::getInstance().LOG("Invalid inputs. Try setting IP and PORT.");
        return;
    }

    if (!m_engine->isValid()) {
        Logger::getInstance().LOG("Engine not valid. Aborting...");
        return;
    }

    //parse addrs
    QHostAddress qha_localAddr(QHostAddress::Any);
    int localport = 8888;

    QHostAddress qha_targetAddr(m_targetIP);

    //socket
    if (!m_sock.bind(qha_localAddr, localport)) {
        Logger::getInstance().LOGF("Failed to bind socket on %d. Aborting...", localport);
        cleanup();
        return;
    }

    QObject::connect(&m_sock, &QUdpSocket::readyRead, [=]{
        util::read_socket(this);
    });

    //set up natives
    if (!util::QHAToAddress(qha_localAddr, localport, &m_localAddr)) {
        Logger::getInstance().LOG("Failed to convert local QHostAddress to native address. Aborting...");
        disconnect();
        return;
    }

    if (!util::QHAToAddress(qha_targetAddr, m_targetPort, &m_targetAddr)) {
        Logger::getInstance().LOG("Failed to convert target QHostAddress to native address. Aborting...");
        disconnect();
        return;
    }

    Logger::getInstance().LOG("Trying to connect to " + m_targetIP + ":" + m_targetPort + "...");

    m_conn = lsquic_engine_connect((*m_engine).engine(),            //engine
                                   LSQVER_043,                      //version
                                   (sockaddr*) &m_localAddr,        //local sockaddr
                                   (sockaddr*) &m_targetAddr,       //peer sockaddr
                                   nullptr,                         //peer ctx
                                   nullptr,                         //conn ctx
                                   "psr5qrh59.vm.paperspace.com",   //sni
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
        //TODO
    }
    cleanup();
}


}
}


