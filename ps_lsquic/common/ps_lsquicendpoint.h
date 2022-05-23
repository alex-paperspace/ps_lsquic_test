#ifndef PS_LSQUICENDPOINT_H
#define PS_LSQUICENDPOINT_H

#include "ps_lsquicengine.h"
#include "lsquic.h"
#ifndef Q_OS_WIN
#include "arpa/inet.h"
#else
//#include "winsock2.h"
#endif

#include <QAbstractSocket>
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

    //net
    sockaddr_in m_local_sa;
    QSharedPointer<QAbstractSocket> m_sock;
    int m_fd = 0;

    //event
    event_base* m_ebase;
    event* m_timer;

    void cleanup();

public:
    explicit PS_LSQuicEndpoint();
    virtual ~PS_LSQuicEndpoint();


    virtual bool isServer() = 0;

    int getSockFD();
    void process_conns();
    event_base *ebase() const;
};

namespace util {

int packets_out (void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count);
void read_socket (evutil_socket_t, short, void* ctx);

}

}
}


#endif // PS_LSQUICENDPOINT_H
