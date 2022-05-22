#include "ps_lsquicendpoint.h"
#include "client/ps_lsquicclient.h"
#include "common/logger.h"

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTimer>
//libevent usage is unfinished. need to run loop in separate thread?
//or switch to QTimer singleshots, but no microsecond accuracy
//#define PS_USE_LIBEVENT

namespace paperspace {
namespace lsquic {

static void timer_expired(int, short, void* ctx) {
    PS_LSQuicEndpoint* client = static_cast<PS_LSQuicEndpoint*>(ctx);
    client->process_conns();
}

PS_LSQuicEndpoint::PS_LSQuicEndpoint()
{
#ifdef PS_USE_LIBEVENT
    m_ebase = event_base_new();
    m_timer = event_new(m_ebase, -1, 0, timer_expired, this);
#endif
}

PS_LSQuicEndpoint::~PS_LSQuicEndpoint()
{
#ifdef PS_USE_LIBEVENT
    event_base_loopbreak(m_ebase);
    event_free(m_timer);
    event_base_free(m_ebase);
#endif
}

int PS_LSQuicEndpoint::getSockFD()
{
    //return m_fd;
    if (m_sock.isNull()) {
        return -1;
    }

    return (*m_sock).socketDescriptor();
}


void PS_LSQuicEndpoint::process_conns()
{
    Logger::getInstance().LOG("Processing connections.");
#ifdef PS_USE_LIBEVENT
    event_del(m_timer);
#endif
    lsquic_engine_process_conns(m_engine->engine());

    int diff;
    timeval timeout;
    if (lsquic_engine_earliest_adv_tick(m_engine->engine(), &diff))
    {
        if (diff >= LSQUIC_DF_CLOCK_GRANULARITY) {
            timeout.tv_sec = (unsigned) diff / 1000000;
            timeout.tv_usec = (unsigned) diff % 1000000;
        } else if (diff <= 0) {
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;
        } else {
            timeout.tv_sec = 0;
            timeout.tv_usec = (unsigned) LSQUIC_DF_CLOCK_GRANULARITY / 1000000;
        }

        Logger::getInstance().LOGF("Converted diff %d usec to %ld.%.6d seconds.", diff, timeout.tv_sec, timeout.tv_usec);
#ifdef PS_USE_LIBEVENT
        event_add(m_timer, &timeout);
#endif
    }
}

namespace util {

int packets_out(void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count)
{
    Logger::getInstance().LOG("Packets Out");

    PS_LSQuicEndpoint* ep = static_cast<PS_LSQuicEndpoint*>(packets_out_ctx);
    int sockfd = ep->getSockFD();
    Logger::getInstance().LOGF("Socket FD: %d", sockfd);

    struct msghdr msg;
    unsigned n;
    memset(&msg, 0, sizeof(msg));

    int charsSent = 0;
    for (n = 0; n < count; ++n)
    {
        msg.msg_name       = (void *) specs[n].dest_sa;
        msg.msg_namelen    = sizeof(struct sockaddr_in);
        msg.msg_iov        = specs[n].iov;
        msg.msg_iovlen     = specs[n].iovlen;

        charsSent = sendmsg(sockfd, &msg, 0);
        if (charsSent < 0) {
            Logger::getInstance().LOGF("Failed to sendmsg(), errno: %d", errno);
            break;
        } else {
            Logger::getInstance().LOGF("%d Characters sent.", charsSent);
        }
    }
    return (int) n;
}

}

}
}

