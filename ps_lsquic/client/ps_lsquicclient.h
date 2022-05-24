#ifndef PS_LSQUICCLIENT_H
#define PS_LSQUICCLIENT_H

#include "common/ps_lsquicendpoint.h"
#include "clientcallbacks.h"

#include <QSharedPointer>

#include <QtGlobal>
#ifdef Q_OS_UNIX
#include "unistd.h"
#endif

namespace paperspace {
namespace lsquic {

class PS_LSQuicClient : public PS_LSQuicEndpoint
{
    QString m_targetIP;
    int m_targetPort = -1;

    ClientCallbacks m_cbs;

    //net
    Address m_targetAddr;


public:
    explicit PS_LSQuicClient();
    ~PS_LSQuicClient();

    bool isServer() override { return false; }

    void setIP(QString ip) { m_targetIP = ip; }
    void setPort(QString port) { m_targetPort = port.toInt(); }
    void connect();
    void disconnect();
};

}
}

typedef paperspace::lsquic::PS_LSQuicClient QuicClient;
typedef QSharedPointer<QuicClient> QuicClientShared;


#endif // PS_LSQUICCLIENT_H
