#ifndef PS_LSQUICENDPOINT_H
#define PS_LSQUICENDPOINT_H

#include "ps_lsquicengine.h"

#include "lsquic.h"

namespace paperspace {
namespace lsquic {

class PS_LSQuicEndpoint
{
protected:

    QuicEngineShared m_engine;

public:
    explicit PS_LSQuicEndpoint();

    virtual bool isServer() = 0;
};

namespace util {

int packets_out (void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count);

}

}
}


#endif // PS_LSQUICENDPOINT_H
