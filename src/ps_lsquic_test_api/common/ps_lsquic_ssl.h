#ifndef PS_LSQUIC_SSL_H
#define PS_LSQUIC_SSL_H

#include <QtGlobal>
#include <QMap>
#include <QSharedPointer>

#include <openssl/ssl.h>

#ifndef Q_OS_WIN
#include "sys/socket.h"
#endif

namespace paperspace {
namespace lsquic {

//Even though the library is forcing us to parse SNI and lookup
//a proper cert, we are one machine and should just return the one cert
//that we created

//However these class declarations are structured in a way to allow for multiple certs
//to not be confusing.

class PS_LSQuicServerCert
{
public:
    ssl_ctx_st* ssl_ctx = nullptr;

    ~PS_LSQuicServerCert();
};

typedef QSharedPointer<PS_LSQuicServerCert> ServerCert;

class PS_LSQuicSSL
{

    QMap<QString, ServerCert> m_sniMap;

public:
    explicit PS_LSQuicSSL();
    ~PS_LSQuicSSL();

    ssl_ctx_st* prog_ssl_ctx;

    ServerCert getCertbySNI(QString sni);

    int load_cert(const char* sni, const char *cert_file, const char *key_file);

};

extern "C" ssl_ctx_st* no_cert (void *cert_lu_ctx, const struct sockaddr *sa_UNUSED, const char *sni);

//not sure why i need to use C-style symbol exporting for the "server" program to link successfully to these methods
extern "C" ssl_ctx_st* lookup_cert (void *cert_lu_ctx, const struct sockaddr *sa_UNUSED, const char *sni);

extern "C" ssl_ctx_st* get_ssl_ctx(void *peer_ctx, const sockaddr*);

}
}

#endif // PS_LSQUIC_SSL_H
