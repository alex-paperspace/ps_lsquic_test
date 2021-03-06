#ifndef PS_LSQUICENDPOINT_H
#define PS_LSQUICENDPOINT_H

#include "ps_lsquicengine.h"
#include "lsquic.h"

#include <QtGlobal>
#ifndef Q_OS_WIN
#include "arpa/inet.h"
#endif

#include <QUdpSocket>
#include <QTimer>

namespace paperspace {
namespace lsquic {

typedef sockaddr_storage Address;

class PS_LSQuicEndpoint : public QObject
{

protected:

    //lsquic
    QuicEngineShared m_engine;
    lsquic_engine_api m_eapi;
    lsquic_logger_if m_logIF;
    lsquic_conn* m_conn = nullptr;

    //net
    Address m_localAddr;
    QUdpSocket m_sock;
    int m_fd = 0;

    //timer
    QTimer m_timer;

public:
    explicit PS_LSQuicEndpoint();
    virtual ~PS_LSQuicEndpoint();

    void process_conns();

    virtual bool isServer() = 0;

    QUdpSocket& socket() { return m_sock; }
    Address& getLocalAddress() { return m_localAddr; }
    lsquic_engine* engine() const;

    QByteArray recvbuf;

    void cleanup();

    //disconnects socket signals, used for connection closure
    void disconnectSocket();
};

} // lsquic
} // paperspace


#endif // PS_LSQUICENDPOINT_H
