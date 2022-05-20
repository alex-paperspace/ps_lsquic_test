#ifndef CLIENTCALLBACKS_H
#define CLIENTCALLBACKS_H

#include "lsquic.h"

namespace paperspace {
namespace lsquic {

class ClientCallbacks
{
    struct lsquic_stream_if m_interface;

public:
    ClientCallbacks();

    lsquic_stream_if* getInterface() { return &m_interface; }
};

}
}


#endif // CLIENTCALLBACKS_H
