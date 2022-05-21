#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <iostream>
#include <QObject>
#include <QString>

namespace paperspace {
namespace lsquic {

class Logger : public QObject
{
    Q_OBJECT

    explicit Logger(QObject* parent = nullptr) {}
    Logger(Logger const&);
    void operator=(Logger const&);
public:

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void LOG(const QString& str);
    //void LOG(const std::string& str);

signals:
    void textSignal(QString);
};

}
}



#endif // LOGGER_H
