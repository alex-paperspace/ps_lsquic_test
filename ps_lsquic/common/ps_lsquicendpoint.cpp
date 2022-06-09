#include "ps_lsquicendpoint.h"
#include "client/ps_lsquicclient.h"
#include "common/logger.h"
#include "common/ps_lsquicutil.h"

#include <QtGlobal>
#ifdef Q_OS_WIN
#include "vc_compat.h"
#endif

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTimer>

namespace paperspace {
namespace lsquic {

PS_LSQuicEndpoint::PS_LSQuicEndpoint()
{
    m_timer.setSingleShot(true);
    QObject::connect(&m_timer, &QTimer::timeout, [this]{
        util::timer_expired(this);
    });
}

PS_LSQuicEndpoint::~PS_LSQuicEndpoint()
{

}

//TODO: use signals/slots to implement this mechanism, granularity is 1ms anyway
void PS_LSQuicEndpoint::process_conns()
{
    Logger::getInstance().LOG("Processing connections.");
    lsquic_engine_process_conns(this->engine());

    int diff;
    if (lsquic_engine_earliest_adv_tick(m_engine->engine(), &diff))
    {
        if (diff >= LSQUIC_DF_CLOCK_GRANULARITY) {
            m_timer.setInterval(diff / 1000);
        } else if (diff <= 0) {
            util::timer_expired(this);
        } else {
            m_timer.setInterval(1);
        }
        Logger::getInstance().LOGF("Converted diff %d usec to %ld.%.3d seconds.", diff, m_timer.interval() / 1000, m_timer.interval() % 1000);
        m_timer.start();
    }
}

lsquic_engine* PS_LSQuicEndpoint::engine() const
{
    return m_engine->engine();
}

void PS_LSQuicEndpoint::disconnectSocket()
{
    if (!m_sock.disconnect()) {
        Logger::getInstance().LOG("Failed to disconnect socket signals");
    }
}

void PS_LSQuicEndpoint::cleanup()
{
    Logger::getInstance().LOG("Cleaning up.");
    if (m_sock.isValid() || m_sock.isOpen()) {
        m_sock.close();
    }
    if (m_conn) {
        m_conn = nullptr;
    }
}

}
}

