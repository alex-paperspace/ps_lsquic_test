#include "ps_lsquicserver.h"
#include "logger.h"
#include "ps_lsquic_ssl.h"
#include "ps_lsquicutil.h"

#include <QNetworkInterface>

namespace paperspace {
namespace lsquic {

PS_LSQuicServer::PS_LSQuicServer()
{
    memset(&m_eapi, 0, sizeof(m_eapi));

    m_eapi.ea_packets_out = util::packets_out;
    m_eapi.ea_packets_out_ctx = this;

    m_eapi.ea_stream_if = m_cbs.getInterface();
    m_eapi.ea_stream_if_ctx = this;

    m_eapi.ea_lookup_cert = lookup_cert;
    m_eapi.ea_cert_lu_ctx = (void*) &m_ssl;

    m_eapi.ea_get_ssl_ctx = get_ssl_ctx;
    lsquic_engine_settings settings;
    lsquic_engine_init_settings(&settings, LSENG_SERVER);

    settings.es_versions = LSQUIC_GQUIC_HEADER_VERSIONS;
    settings.es_support_tcid0 = false;

    if (-1 == lsquic_engine_check_settings(&settings, LSENG_SERVER, nullptr, 0)) {
        Logger::getInstance().LOG("Engine settings not valid");
    } else {
        m_eapi.ea_settings = &settings;
    }

    m_engine = QuicEngineShared(new PS_LSQuicEngine(m_eapi, true));

    if (-1 == lsquic_set_log_level("debug")) {
        Logger::getInstance().LOG("Failed setting log level");
    }
    m_logIF.log_buf = util::lsquicLogCB;
    lsquic_logger_init(&m_logIF, nullptr, LLTS_HHMMSSMS);
}

PS_LSQuicServer::~PS_LSQuicServer()
{

}

void PS_LSQuicServer::listen()
{
    cleanup();

    if (m_listenPort == -1) {
        Logger::getInstance().LOG("Invalid input. Try setting the port.");
        return;
    }

    if (!m_engine->isValid()) {
        Logger::getInstance().LOG("Engine not valid. Aborting...");
        return;
    }

    //cert
    m_ssl.load_cert("localhost", m_certfileStr, m_keyfileStr);

    unsigned mask = lsquic_engine_quic_versions(engine());
    Logger::getInstance().LOGF("Quic Versions Supported, Mask: %d", mask);

    //local
    QHostAddress qha_localAddr = util::getUsableHostAddress();

    //socket
    if (!m_sock.bind(m_listenPort)) {
        Logger::getInstance().LOGF("Failed to bind socket on %d. Aborting...", m_listenPort);
        cleanup();
        return;
    }

    QObject::connect(&m_sock, &QUdpSocket::readyRead, [this]{
        util::read_socket(this);
    });

    Logger::getInstance().LOGF("Listening on port: %d", m_sock.localPort());

    //natives
    if (!util::QHAToAddress(qha_localAddr, m_listenPort, &m_localAddr)) {
        Logger::getInstance().LOG("Failed to convert local QHostAddress to native address. Aborting...");
        cleanup();
        return;
    }
}


}
}
