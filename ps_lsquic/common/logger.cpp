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

void Logger::LOGF(const char *fmt...)
{
    va_list ap;
    char buf[500];
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    QString str(buf);
    emit textSignal(str);
}

//void Logger::LOG(const std::string &str)
//{
//    emit textSignal(QString::fromStdString(str));
//}

}
}

