#ifndef PS_LSQUICSERVER_H
#define PS_LSQUICSERVER_H

#include "common/ps_lsquicendpoint.h"
#include "servercallbacks.h"

#include <QSharedPointer>
#include <QAbstractSocket>

#include <QtGlobal>
#ifdef Q_OS_UNIX
#include "unistd.h"
#endif

#ifdef Q_OS_WIN
#include  <Mswsock.h>
#endif

namespace paperspace {
namespace lsquic {

class PS_LSQuicServer : public PS_LSQuicEndpoint
{
    QString m_listenPortStr;

    ServerCallbacks m_cbs;

    lsquic_engine_api m_eapi;

    //laziness for now
    const char* m_certfileStr = "mycert-cert.pem";
    const char* m_keyfileStr = "mycert-key.pem";

    sockaddr_in m_local_sa;
    QSharedPointer<QAbstractSocket> m_sock;
    int m_fd = 0;

    //read event
    event* m_readEv;

#ifdef Q_OS_WIN
    LPFN_WSARECVMSG WSARecvMsg = NULL;
#endif

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
