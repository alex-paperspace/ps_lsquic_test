#ifndef PS_LSQUICUTIL_H
#define PS_LSQUICUTIL_H

#include <ps_lsquicendpoint.h>
#include "lsquic.h"

namespace paperspace {
namespace lsquic {
namespace util {

int packets_out (void *packets_out_ctx, const lsquic_out_spec *specs, unsigned count);

void read_socket (PS_LSQuicEndpoint* ep);

void timer_expired (PS_LSQuicEndpoint* ep);

bool QHAToAddress (const QHostAddress& qha, int port, Address* resAddr);

int lsquicLogCB(void* logger_ctx, const char* buf, size_t len);

ssl_ctx_st* no_cert (void *cert_lu_ctx, const struct sockaddr *sa_UNUSED, const char *sni);

} // util
} // lsquic
} // paperspace

#endif // PS_LSQUICUTIL_H
