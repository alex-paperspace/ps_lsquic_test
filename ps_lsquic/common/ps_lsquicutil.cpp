#include "ps_lsquicutil.h"
#include "common/logger.h"

#include <QNetworkDatagram>

namespace paperspace {
namespace lsquic {
namespace util {

int packets_out(void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count)
{
    Logger::getInstance().LOG("Packets Out");

    PS_LSQuicEndpoint* ep = static_cast<PS_LSQuicEndpoint*>(packets_out_ctx);
    int sockfd = ep->getSockFD();
    Logger::getInstance().LOGF("Socket FD: %d", sockfd);

    unsigned n = 0;

    for (n = 0; n < count; ++n)
    {
#ifdef PS_USE_SENDMSG
        int charsSent = 0;
        struct msghdr msg;
        memset(&msg, 0, sizeof(msg));

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
#else
        QNetworkDatagram dg;
        QHostAddress dest(specs[n].dest_sa);
        int port;
        //to get port, we must convert to sockaddr and retrieve port, modify ctx for this
        switch (dest.protocol()) {
            case QAbstractSocket::IPv4Protocol:
            case QAbstractSocket::AnyIPProtocol:
            {
                const sockaddr_in *sin = reinterpret_cast<const sockaddr_in*>(specs[n].dest_sa);
                port = ntohs(sin->sin_port);
                dg.setDestination(dest, port);
                break;
            }
            case QAbstractSocket::IPv6Protocol:
            {
                const sockaddr_in6 *sin6 = reinterpret_cast<const sockaddr_in6*>(specs[n].dest_sa);
                port = ntohs(sin6->sin6_port);
                dg.setDestination(dest, port);
                break;
            }
            case QAbstractSocket::UnknownNetworkLayerProtocol:
            {
                Logger::getInstance().LOG("Unknown Network Layer Protocol. Aborting...");
                return -1;
            }
        }

        QByteArray ba;
        char * pCurr = (char*)specs[n].iov->iov_base;
        for (size_t i = 0; i < specs[n].iovlen; ++i) {
            ba.append(pCurr, specs[n].iov->iov_len);
            pCurr += specs[n].iov->iov_len;
        }
        dg.setData(ba);

        int bytessent = ep->socket().writeDatagram(ba, dest, port);
        if (bytessent == -1) {
            Logger::getInstance().LOGF("Failed to send datagram. error-#: %d error-string: %s Aborting...", ep->socket().error(), ep->socket().errorString().toStdString().c_str());
            return -1;
        } else {
            Logger::getInstance().LOGF("%d bytes sent.", bytessent);
        }
#endif

    }

    return (int) n;
}


void read_socket(PS_LSQuicEndpoint* ep)
{
    Logger::getInstance().LOG("Reading socket..");

    while (ep->socket().hasPendingDatagrams()) {
        QNetworkDatagram dg = ep->socket().receiveDatagram();

        const std::size_t count = dg.data().size();
        unsigned char* payload = new unsigned char[count]; //do i have to delete this at some point?
        memcpy(payload, dg.data().constData(), count);

        //get peer address
        Address peerAddr;
        if (!util::QHAToAddress(dg.senderAddress(), dg.senderPort(), &peerAddr)) {
            Logger::getInstance().LOG("Failed to convert sender QHostAddress to native address. Dropping/Continuing...");
            continue;
        }

        int success = lsquic_engine_packet_in(ep->engine(),
                                              payload,
                                              count,
                                              (sockaddr*) &ep->getLocalAddress(),
                                              (sockaddr*) &peerAddr,
                                              nullptr,
                                              0);

        if (success == 0) {
            Logger::getInstance().LOG("Processed by a real conn");
        } else if (success == 1) {
            Logger::getInstance().LOG("Handled, but not by a conn");
        } else if (success == -1) {
            Logger::getInstance().LOG("Error occurred passing packet in");
        }

        ep->process_conns();
    }

}

void timer_expired(PS_LSQuicEndpoint *ep)
{
    ep->process_conns();
}

//for converting a QHostAddress to an Address
//used mostly when we want to supply sockaddrs to lsquic
bool QHAToAddress(const QHostAddress &qha, int port, Address* resAddr)
{
    memset(resAddr, 0, sizeof(Address));
    switch (qha.protocol()) {
        case QAbstractSocket::IPv4Protocol:
        case QAbstractSocket::AnyIPProtocol:
        {
            sockaddr_in sa;
            QString ipStr = qha.toString();
            QByteArray ipBA = ipStr.toLocal8Bit();

            Logger::getInstance().LOGF("IPV4 Address detected: %s", ipStr.toStdString().c_str());

            sa.sin_family = AF_INET;
            inet_pton(AF_INET, ipBA.data(), &sa.sin_addr);
            sa.sin_port = htons(port);

            resAddr->addr4 = sa;

            return true;
        }
        case QAbstractSocket::IPv6Protocol:
        {
            sockaddr_in6 sa;
            QString ipStr = qha.toString();
            QByteArray ipBA = ipStr.toLocal8Bit();

            Logger::getInstance().LOGF("IPV6 Address detected: %s", ipStr.toStdString().c_str());

            sa.sin6_family = AF_INET6;
            inet_pton(AF_INET6, ipBA.data(), &sa.sin6_addr);
            sa.sin6_port = htons(port);

            resAddr->addr6 = sa;

            return true;
        }
        case QAbstractSocket::UnknownNetworkLayerProtocol:
        {
            Logger::getInstance().LOG("Unknown Network Layer Protocol. Aborting...");

            return false;
        }
    }
}

int lsquicLogCB(void *, const char *buf, size_t len) {
    fprintf(stderr, "%s\n", buf);
    Logger::getInstance().LOGF("%s", buf);
    return len;
}

} // util
} // lsquic
} // paperspace
