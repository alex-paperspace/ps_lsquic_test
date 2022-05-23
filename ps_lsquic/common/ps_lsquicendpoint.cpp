#include "ps_lsquicendpoint.h"
#include "client/ps_lsquicclient.h"
#include "common/logger.h"

#include <QtGlobal>
#ifdef Q_OS_WIN
#include "vc_compat.h"
#endif

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTimer>

//libevent usage is unfinished. need to run loop in separate thread?
//#define PS_USE_LIBEVENT

namespace paperspace {
namespace lsquic {

static void timer_expired(evutil_socket_t, short, void* ctx) {
    PS_LSQuicEndpoint* client = static_cast<PS_LSQuicEndpoint*>(ctx);
    client->process_conns();
}

PS_LSQuicEndpoint::PS_LSQuicEndpoint()
{
    m_ebase = event_base_new();
#ifdef PS_USE_LIBEVENT
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

event_base *PS_LSQuicEndpoint::ebase() const
{
    return m_ebase;
}

void PS_LSQuicEndpoint::cleanup()
{
    Logger::getInstance().LOG("Cleaning up.");
    if (!m_sock.isNull()) {
        m_sock.clear();
        m_fd = -1;
    }
}

namespace util {

int packets_out(void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count)
{
    Logger::getInstance().LOG("Packets Out");

    PS_LSQuicEndpoint* ep = static_cast<PS_LSQuicEndpoint*>(packets_out_ctx);
    int sockfd = ep->getSockFD();
    Logger::getInstance().LOGF("Socket FD: %d", sockfd);


    unsigned n = 0;
#ifdef Q_OS_UNIX

    struct msghdr msg;
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


#endif

#ifdef Q_OS_WIN

#endif

    return (int) n;
}


void read_socket(evutil_socket_t, short, void *ctx)
{

    PS_LSQuicEndpoint* ep = static_cast<PS_LSQuicEndpoint*>(ctx);
    //event_base_loopbreak(ep->ebase());

    Logger::getInstance().LOGF("Read socket, fd: %d", ep->getSockFD());


//    int nread = WSARecvMsg;



//    (void) lsquic_engine_packet_in(tut->tut_engine, buf, nread,
//        (struct sockaddr *) &local_sas,
//        (struct sockaddr *) &peer_sas,
//        (void *) (uintptr_t) w->fd, ecn);

}

}

}
}

