#ifndef PS_LSQUICSERVER_H
#define PS_LSQUICSERVER_H

#include "common/ps_lsquicendpoint.h"
#include "servercallbacks.h"

#include <QSharedPointer>
#include <QAbstractSocket>

namespace paperspace {
namespace lsquic {

class PS_LSQuicServer : public PS_LSQuicEndpoint
{
    QString m_listenPortStr;

    ServerCallbacks m_cbs;

    lsquic_engine_api m_eapi;

    //laziness for now
    const char* m_certfileStr = "/Users/alexmalynovsky/projects/ps_lsquic_test/mycert-cert.pem";
    const char* m_keyfileStr = "/Users/alexmalynovsky/projects/ps_lsquic_test/mycert-key.pem";

    sockaddr_in m_local_sa;
    QSharedPointer<QAbstractSocket> m_sock;
    int m_fd = 0;

    lsquic_conn* m_conn;



public:
    explicit PS_LSQuicServer();

    bool isServer() override { return true; }

    void setListenPort(QString portStr) { m_listenPortStr = portStr; }
    void listen();
};

}
}

typedef paperspace::lsquic::PS_LSQuicServer QuicServer;
typedef QSharedPointer<QuicServer> QuicServerShared;

#endif // PS_LSQUICSERVER_H
