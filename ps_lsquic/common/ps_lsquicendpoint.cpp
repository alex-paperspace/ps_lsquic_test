#include "ps_lsquicendpoint.h"


namespace paperspace {
namespace lsquic {

PS_LSQuicEndpoint::PS_LSQuicEndpoint()
{

}

namespace util {

int packets_out(void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count)
{
    return 0;
}

}

}
}

