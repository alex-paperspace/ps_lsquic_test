#include "logger.h"
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>


namespace paperspace {
namespace lsquic {

void Logger::LOG(const QString &str)
{
    emit textSignal(str);
}

//void Logger::LOG(const std::string &str)
//{
//    emit textSignal(QString::fromStdString(str));
//}

}
}

