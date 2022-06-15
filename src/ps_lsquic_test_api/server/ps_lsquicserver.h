#ifndef PS_LSQUICSERVER_H
#define PS_LSQUICSERVER_H

#include "ps_lsquicendpoint.h"
#include "servercallbacks.h"
#include "ps_lsquic_ssl.h"

#include <QSharedPointer>
#include <QAbstractSocket>

#include <QtGlobal>
#ifdef Q_OS_UNIX
#include "unistd.h"
#endif

namespace paperspace {
namespace lsquic {

class PS_LSQuicServer : public PS_LSQuicEndpoint
{
    int m_listenPort = -1;

    ServerCallbacks m_cbs;

    PS_LSQuicSSL m_ssl;

    //laziness for now
    const char* m_certfileStr = "cert.pem";
    const char* m_keyfileStr = "key.pem";

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
