#ifndef PS_LSQUICENDPOINT_H
#define PS_LSQUICENDPOINT_H

#include "ps_lsquicengine.h"

#include "lsquic.h"

#include <QtGlobal>
#include <event2/event.h>
#ifndef Q_OS_WIN
#include "arpa/inet.h"
#else
#include "winsock2.h"
#include "wstcpip2.h"
#endif

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

    //event
    event_base* m_ebase;
    event* m_timer;

public:
    explicit PS_LSQuicEndpoint();
    virtual ~PS_LSQuicEndpoint();


    virtual bool isServer() = 0;

    void process_conns();
};

namespace util {

int packets_out (void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count);

}

}
}


#endif // PS_LSQUICENDPOINT_H
