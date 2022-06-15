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
    bool m_isValid = false;

public:
    explicit PS_LSQuicEngine(const lsquic_engine_api& api, bool isServer);

    bool isValid() { return m_isValid; }

    lsquic_engine* engine() { return m_engine; }

};

typedef QSharedPointer<PS_LSQuicEngine> QuicEngineShared;

}
}



#endif // PS_LSQUICENGINE_H
