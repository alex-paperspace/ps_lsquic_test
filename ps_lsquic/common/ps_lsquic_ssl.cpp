#include "ps_lsquic_ssl.h"

#include <iostream>
#include "common/logger.h"

namespace paperspace {
namespace lsquic {

int PS_LSQuicSSL::load_cert(const char *cert_file, const char *key_file)
{
    int rv = -1;

    m_ssl_ctx = SSL_CTX_new(TLS_method());
    if (!m_ssl_ctx)
    {
        Logger::getInstance().LOG("SSL_CTX_new failed");
        goto end;
    }
    SSL_CTX_set_min_proto_version(m_ssl_ctx, TLS1_3_VERSION);
    SSL_CTX_set_max_proto_version(m_ssl_ctx, TLS1_3_VERSION);
    SSL_CTX_set_default_verify_paths(m_ssl_ctx);
    if (1 != SSL_CTX_use_certificate_chain_file(m_ssl_ctx, cert_file))
    {
        Logger::getInstance().LOG("SSL_CTX_use_certificate_chain_file failed");
        goto end;
    }
    if (1 != SSL_CTX_use_PrivateKey_file(m_ssl_ctx, key_file,
                                                            SSL_FILETYPE_PEM))
    {
        Logger::getInstance().LOG("SSL_CTX_use_PrivateKey_file failed");
        goto end;
    }
    rv = 0;

  end:
    if (rv != 0)
    {
        if (m_ssl_ctx)
            SSL_CTX_free(m_ssl_ctx);
        m_ssl_ctx = NULL;
    }
    return rv;
}

SSL_CTX *get_ssl_ctx(void *peer_ctx, const sockaddr*)
{
    Logger::getInstance().LOG("Get SSL_CTX");
    return PS_LSQuicSSL::getInstance().m_ssl_ctx;
}

}
}


