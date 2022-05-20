#ifndef PS_LSQUICSERVER_H
#define PS_LSQUICSERVER_H

#include "ps_lsquicendpoint.h"

namespace paperspace {
namespace lsquic {

class PS_LSQuicServer : public PS_LSQuicEndpoint
{
public:
    explicit PS_LSQuicServer();

    bool isServer() { return true; }
};

}
}


#endif // PS_LSQUICSERVER_H
