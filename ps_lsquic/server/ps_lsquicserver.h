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
    int m_listenPort = -1;

    ServerCallbacks m_cbs;

    //laziness for now
    const char* m_certfileStr = "mycert-cert.pem";
    const char* m_keyfileStr = "mycert-key.pem";

#ifdef Q_OS_WIN
    LPFN_WSARECVMSG WSARecvMsg = NULL;
#endif

public:
    explicit PS_LSQuicServer();
    ~PS_LSQuicServer();

    bool isServer() override { return true; }

    void setListenPort(QString portStr) { m_listenPort = portStr.toInt(); }
    void listen();
};

}
}

typedef paperspace::lsquic::PS_LSQuicServer QuicServer;
typedef QSharedPointer<QuicServer> QuicServerShared;

#endif // PS_LSQUICSERVER_H
