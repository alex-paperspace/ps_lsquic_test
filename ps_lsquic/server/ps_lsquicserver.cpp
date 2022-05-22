#include "ps_lsquicserver.h"
#include "common/logger.h"
#include "common/ps_lsquic_ssl.h"

#include <unistd.h>

namespace paperspace {
namespace lsquic {

PS_LSQuicServer::PS_LSQuicServer()
{
    if (0 != PS_LSQuicSSL::getInstance().load_cert(m_certfileStr, m_keyfileStr)) {
        Logger::getInstance().LOG("Couldn't load cert");
    }

    memset(&m_eapi, 0, sizeof(m_eapi));

    m_eapi.ea_packets_out = util::packets_out;
    m_eapi.ea_packets_out_ctx = this;

    m_eapi.ea_stream_if = m_cbs.getInterface();
    m_eapi.ea_stream_if_ctx = nullptr;

    //PS_LSQuicSSL::getInstance().load_cert("","");

    m_eapi.ea_get_ssl_ctx = get_ssl_ctx;
    m_eapi.ea_settings = NULL; //use defaults, can change later

    m_engine = QuicEngineShared(new PS_LSQuicEngine(m_eapi, true));

}

void PS_LSQuicServer::listen()
{
    //cleanup();

    if (m_listenPortStr.isEmpty()) {
        Logger::getInstance().LOG("Empty input. Aborting...");
        return;
    }

    if (!m_engine->isValid()) {
        Logger::getInstance().LOG("Engine not valid. Aborting...");
        return;
    }

    m_local_sa.sin_family = AF_INET;
    m_local_sa.sin_addr.s_addr = INADDR_ANY;
    m_local_sa.sin_port = m_listenPortStr.toInt();

    Logger::getInstance().LOGF("Trying to listen on port: %d", m_local_sa.sin_port);

    //socket
    m_fd = socket(m_local_sa.sin_family, SOCK_DGRAM, 0);

    int socklen = sizeof(m_local_sa);
    if (0 != bind(m_fd, (sockaddr*)&m_local_sa, socklen)) {
        Logger::getInstance().LOGF("Failed to bind socket on %d. errno %d Aborting...", m_fd, errno);
        cleanup();
        return;
    }
    Logger::getInstance().LOGF("Bound socket to fd: %d", m_fd);

    m_sock = QSharedPointer<QAbstractSocket>(new QAbstractSocket(QAbstractSocket::UdpSocket, nullptr));
    m_sock->setSocketDescriptor(m_fd, QAbstractSocket::UnconnectedState);

    QObject::connect(m_sock.data(), &QAbstractSocket::readyRead, [=]{
        Logger::getInstance().LOG("Stuff in socket");
    });

    Logger::getInstance().LOGF("Listening on port: %d", m_local_sa.sin_port);

}


}
}
