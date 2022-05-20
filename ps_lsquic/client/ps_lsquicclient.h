#ifndef PS_LSQUICCLIENT_H
#define PS_LSQUICCLIENT_H

#include "common/ps_lsquicendpoint.h"
#include "clientcallbacks.h"
#include "common/ps_lsquic_ssl.h"

#include <QSharedPointer>

namespace paperspace {
namespace lsquic {

class PS_LSQuicClient : public PS_LSQuicEndpoint
{
    QString m_targetIPStr;
    QString m_targetPortStr;

    ClientCallbacks m_cbs;

    lsquic_engine_api m_eapi;

public:
    explicit PS_LSQuicClient();
    virtual ~PS_LSQuicClient() {}

    bool isServer() { return false; }

    void setIP(QString ip) { m_targetIPStr = ip; }
    void setPort(QString port) { m_targetPortStr = port; }
    void connect();
};

}
}

typedef paperspace::lsquic::PS_LSQuicClient QuicClient;
typedef QSharedPointer<QuicClient> QuicClientShared;


#endif // PS_LSQUICCLIENT_H
