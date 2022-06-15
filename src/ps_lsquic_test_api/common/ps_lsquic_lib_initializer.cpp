#include "ps_lsquic_lib_initializer.h"

#include <stdlib.h>
#include <iostream>

#include "lsquic.h"

namespace paperspace {
namespace lsquic {

bool initialize_lsquic()
{
    if (0 != lsquic_global_init(LSQUIC_GLOBAL_SERVER|LSQUIC_GLOBAL_CLIENT))
    {
        std::cerr << "global lsquic initialization failed" << std::endl;
        return false;
    }
    return true;
}

}
}


