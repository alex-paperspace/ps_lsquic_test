#include "ps_lsquicclient.h"

namespace paperspace {
namespace lsquic {

PS_LSQuicClient::PS_LSQuicClient()
{
    memset(&m_eapi, 0, sizeof(m_eapi));

    m_eapi.ea_packets_out = util::packets_out;
    m_eapi.ea_packets_out_ctx = nullptr;

    m_eapi.ea_stream_if = m_cbs.getInterface();
    m_eapi.ea_stream_if_ctx = nullptr;

    PS_LSQuicSSL::getInstance().load_cert("","");

    m_eapi.ea_get_ssl_ctx = get_ssl_ctx;
    m_eapi.ea_settings = NULL; //use defaults, can change later

    m_engine = QuicEngineShared(new PS_LSQuicEngine(m_eapi, false));
}

void PS_LSQuicClient::connect()
{

}

}
}


