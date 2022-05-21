#include "clientcallbacks.h"

#include "common/logger.h"

namespace paperspace {
namespace lsquic {

static lsquic_conn_ctx_t *
client_on_new_conn (void *stream_if_ctx, struct lsquic_conn *conn)
{
    Logger::getInstance().LOG("New connection");
}

static void
client_on_conn_closed (struct lsquic_conn *conn)
{
    Logger::getInstance().LOG("Connection closed");
}

static lsquic_stream_ctx_t *
client_on_new_stream (void *stream_if_ctx, struct lsquic_stream *stream)
{
    Logger::getInstance().LOG("New stream");
}

static void
client_on_read (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("On read");
}

static void
client_on_write (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("On write");
}

static void
client_on_close (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("Stream closed");
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

