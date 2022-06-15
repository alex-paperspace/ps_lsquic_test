#ifndef SERVERCALLBACKS_H
#define SERVERCALLBACKS_H

#include "lsquic.h"

namespace paperspace {
namespace lsquic {

class ServerCallbacks
{
    struct lsquic_stream_if m_interface;

public:
    ServerCallbacks();

    lsquic_stream_if* getInterface() { return &m_interface; }
};

}
}

#endif // SERVERCALLBACKS_H
