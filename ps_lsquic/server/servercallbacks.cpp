#include "servercallbacks.h"

#include "common/logger.h"

namespace paperspace {
namespace lsquic {

static lsquic_conn_ctx_t *
server_on_new_conn (void *stream_if_ctx, struct lsquic_conn *conn)
{
    Logger::getInstance().LOG("New connection");
}

static void
server_on_conn_closed (struct lsquic_conn *conn)
{
    Logger::getInstance().LOG("Connection closed");
}

static lsquic_stream_ctx_t *
server_on_new_stream (void *stream_if_ctx, struct lsquic_stream *stream)
{
    Logger::getInstance().LOG("New stream");
}

static void
server_on_read (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("On read");
}

static void
server_on_write (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("On write");
}

static void
server_on_close (struct lsquic_stream *stream, lsquic_stream_ctx_t *h)
{
    Logger::getInstance().LOG("Stream closed");
}


ServerCallbacks::ServerCallbacks()
{
    m_interface.on_new_conn = server_on_new_conn;
    m_interface.on_conn_closed = server_on_conn_closed;
    m_interface.on_new_stream = server_on_new_stream;
    m_interface.on_read = server_on_read;
    m_interface.on_write = server_on_write;
    m_interface.on_close = server_on_close;
}

}
}
