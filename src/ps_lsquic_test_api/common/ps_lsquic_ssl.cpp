#include "ps_lsquic_ssl.h"

#include <iostream>
#include "logger.h"

namespace paperspace {
namespace lsquic {

PS_LSQuicServerCert::~PS_LSQuicServerCert()
{
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
        ssl_ctx = nullptr;
    }
}

PS_LSQuicSSL::PS_LSQuicSSL()
{
    prog_ssl_ctx = SSL_CTX_new(TLS_method());
    if (!prog_ssl_ctx) {
        Logger::getInstance().LOG("SSL_CTX_new failed");
    } else {
//        SSL_CTX_set_min_proto_version(prog_ssl_ctx, TLS1_3_VERSION);
//        SSL_CTX_set_max_proto_version(prog_ssl_ctx, TLS1_3_VERSION);
//        SSL_CTX_set_default_verify_paths(prog_ssl_ctx);
    }


}

PS_LSQuicSSL::~PS_LSQuicSSL()
{
    if (prog_ssl_ctx) {
        SSL_CTX_free(prog_ssl_ctx);
        prog_ssl_ctx = nullptr;
    }
}

ServerCert PS_LSQuicSSL::getCertbySNI(QString sni)
{
    return m_sniMap[sni];
}

int PS_LSQuicSSL::load_cert(const char* sni, const char *cert_file, const char *key_file)
{
    int rv = -1;
    ServerCert _servercert = ServerCert(new PS_LSQuicServerCert);
    QString sniStr(sni);

    _servercert->ssl_ctx = SSL_CTX_new(TLS_method());
    if (!_servercert->ssl_ctx) {
        Logger::getInstance().LOG("SSL_CTX_new failed");
        goto end;
    }

    //not really exactly sure if these are necessary
//    SSL_CTX_set_min_proto_version(_servercert->ssl_ctx, TLS1_3_VERSION);
//    SSL_CTX_set_max_proto_version(_servercert->ssl_ctx, TLS1_3_VERSION);
//    SSL_CTX_set_default_verify_paths(_servercert->ssl_ctx);

    if (1 != SSL_CTX_use_certificate_chain_file(_servercert->ssl_ctx, cert_file)) {
        Logger::getInstance().LOG("SSL_CTX_use_certificate_chain_file failed");
        goto end;
    }
    //modify this to accept more filetypes
    if (1 != SSL_CTX_use_PrivateKey_file(_servercert->ssl_ctx, key_file,
                                                            SSL_FILETYPE_PEM))
    {
        Logger::getInstance().LOG("SSL_CTX_use_PrivateKey_file failed");
        goto end;
    }
    rv = 0;

    m_sniMap[sniStr] = _servercert;

  end:
    if (rv != 0)
    {
        if (_servercert->ssl_ctx)
            SSL_CTX_free(_servercert->ssl_ctx);
        _servercert->ssl_ctx = NULL;
    }
    return rv;
}

ssl_ctx_st *no_cert(void *cert_lu_ctx, const sockaddr *sa_UNUSED, const char *sni)
{
    return NULL;
}

ssl_ctx_st *lookup_cert(void *cert_lu_ctx, const sockaddr *sa_UNUSED, const char *sni)
{
    Logger::getInstance().LOG("Cert Lookup");
    PS_LSQuicSSL* ssl = static_cast<PS_LSQuicSSL*>(cert_lu_ctx);
    QString sniStr(sni);
    /* We cannot use the sni provided by the client.
     * That is solely used by the quic-proxy to do ip lookup.
     * When a server loads a cert, it always loads it into key=localhost.
     * So when we lookup the cert, we have to specify the cert located at key=localhost.
     */
    return ssl->getCertbySNI("localhost")->ssl_ctx;
}

ssl_ctx_st *get_ssl_ctx(void *peer_ctx, const sockaddr*)
{
    Logger::getInstance().LOG("Get SSL_CTX");
    PS_LSQuicSSL* ssl = static_cast<PS_LSQuicSSL*>(peer_ctx);
    return ssl->prog_ssl_ctx;
}

}
}


