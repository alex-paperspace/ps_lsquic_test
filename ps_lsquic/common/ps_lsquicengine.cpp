#include "ps_lsquicengine.h"
#include "logger.h"

#include <iostream>

namespace paperspace {
namespace lsquic {

PS_LSQuicEngine::PS_LSQuicEngine(const lsquic_engine_api& eapi, bool isServer)
{
        m_engine = lsquic_engine_new(isServer ? LSENG_SERVER : 0, &eapi);

        if (!m_engine) {
            std::cerr << "Engine create failed." << std::endl;
        } else {
            std::cerr << "Engine create succeeded." << std::endl;
        }
}

}
}
