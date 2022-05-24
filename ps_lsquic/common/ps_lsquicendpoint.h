#ifndef PS_LSQUICENDPOINT_H
#define PS_LSQUICENDPOINT_H

#include "ps_lsquicengine.h"
#include "lsquic.h"
#ifndef Q_OS_WIN
#include "arpa/inet.h"
#else
//#include "winsock2.h"
#endif

#include <QUdpSocket>
#include <QtGlobal>

#include <event2/event.h>

namespace paperspace {
namespace lsquic {

union Address {
    sockaddr sa;
    sockaddr_in addr4;
    sockaddr_in6 addr6;
};

class PS_LSQuicEndpoint
{
protected:

    QuicEngineShared m_engine;

    lsquic_engine_api m_eapi;

    lsquic_logger_if m_logIF;

    //net
    Address m_localAddr;
    QUdpSocket m_sock;
    int m_fd = 0;

    lsquic_conn* m_conn = nullptr;

    void cleanup();

public:
    explicit PS_LSQuicEndpoint();
    virtual ~PS_LSQuicEndpoint();

    void process_conns();

    virtual bool isServer() = 0;

    int getSockFD();
    QUdpSocket& socket() { return m_sock; }
    Address& getLocalAddress() { return m_localAddr; }
    lsquic_engine* engine() const;
};

namespace util {

    int packets_out (void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count);

    void read_socket (PS_LSQuicEndpoint* ep);

    bool QHAToAddress (const QHostAddress& qha, int port, Address* resAddr);

    int lsquicLogCB(void* logger_ctx, const char* buf, size_t len);

} // util

} // lsquic
} // paperspace


#endif // PS_LSQUICENDPOINT_H
