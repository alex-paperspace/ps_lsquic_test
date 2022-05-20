#ifndef PS_LSQUICENGINE_H
#define PS_LSQUICENGINE_H

#include "lsquic.h"

#include <QSharedPointer>

namespace paperspace {
namespace lsquic {

class PS_LSQuicEngine
{
protected:

    lsquic_engine* m_engine;

public:
    explicit PS_LSQuicEngine(const lsquic_engine_api& api, bool isServer);

};

typedef QSharedPointer<PS_LSQuicEngine> QuicEngineShared;

}
}



#endif // PS_LSQUICENGINE_H
