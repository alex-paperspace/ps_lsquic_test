#include "ps_lsquicengine.h"
#include "logger.h"

#include <iostream>

namespace paperspace {
namespace lsquic {

PS_LSQuicEngine::PS_LSQuicEngine(const lsquic_engine_api& eapi, bool isServer)
{
    m_engine = lsquic_engine_new(isServer ? LSENG_SERVER : 0, &eapi);

    if (!m_engine) {
        Logger::getInstance().LOG("Engine create failed.");
    } else {
        Logger::getInstance().LOG("Engine create succeeded.");
        m_isValid = true;
    }
}

}
}
