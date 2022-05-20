#ifndef PS_LSQUIC_SSL_H
#define PS_LSQUIC_SSL_H

#include <QtGlobal>

#include <openssl/ssl.h>

#ifndef Q_OS_WIN
#include "sys/socket.h"
#else
#include "winsock2.h"
#endif

namespace paperspace {
namespace lsquic {

class PS_LSQuicSSL
{
private:
    PS_LSQuicSSL() {}

    PS_LSQuicSSL(PS_LSQuicSSL const&);
    void operator=(PS_LSQuicSSL const&);

public:
    static PS_LSQuicSSL& getInstance()
    {
        static PS_LSQuicSSL instance;
        return instance;
    }

    int load_cert(const char *cert_file, const char *key_file);

    SSL_CTX* m_ssl_ctx;

};

SSL_CTX* get_ssl_ctx(void *peer_ctx, const sockaddr*);

}
}

#endif // PS_LSQUIC_SSL_H
