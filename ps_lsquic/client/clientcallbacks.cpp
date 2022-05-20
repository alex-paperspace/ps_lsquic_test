#include "clientcallbacks.h"

#include <iostream>

namespace paperspace {
namespace lsquic {

static lsquic_conn_ctx_t *
client_on_new_conn (void *stream_if_ctx, struct lsquic_conn *conn)
{
    std::cout << "New connection" << std::endl;
}

static void
client_on_conn_closed (struct lsquic_conn *conn)
{
    std::cout << "Connection closed" << std::endl;
}

static lsquic_stream_ctx_t *
client_on_new_stream (void *stream_if_ctx, struct lsquic_stream *stream)
{
    std::cout << "New stream" << std::endl;
}

static void
client_on_read (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    std::cout << "On read" << std::endl;
}

static void
client_on_write (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    std::cout << "On write" << std::endl;
}

static void
client_on_close (struct lsquic_stream *stream, lsquic_stream_ctx_t *h)
{
    std::cout << "Stream closed" << std::endl;
}


ClientCallbacks::ClientCallbacks()
{
    m_interface.on_new_conn = client_on_new_conn;
    m_interface.on_conn_closed = client_on_conn_closed;
    m_interface.on_new_stream = client_on_new_stream;
    m_interface.on_read = client_on_read;
    m_interface.on_write = client_on_write;
    m_interface.on_close = client_on_close;
}

}
}

