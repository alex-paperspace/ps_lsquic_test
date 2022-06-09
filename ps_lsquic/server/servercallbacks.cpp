#include "servercallbacks.h"
#include "ps_lsquicserver.h"
#include "common/logger.h"

namespace paperspace {
namespace lsquic {

static lsquic_conn_ctx_t *
server_on_new_conn (void *stream_if_ctx, struct lsquic_conn *conn)
{
    Logger::getInstance().LOG("New connection");
    return static_cast<lsquic_conn_ctx_t*>(stream_if_ctx);
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
    if (!stream) {
        Logger::getInstance().LOG("Stream is null, conn going away");
        return nullptr;
    }

    QuicServer* server = static_cast<QuicServer*>((void*)stream_if_ctx);
    if (!server) {
        Logger::getInstance().LOG("Context invalid, aborting stream");
        lsquic_conn_abort(lsquic_stream_conn(stream));
        return nullptr;
    }

    server->recvbuf.clear();
    lsquic_stream_wantread(stream, 1);
    return static_cast<lsquic_stream_ctx_t*>(stream_if_ctx);
}

static void
server_on_read (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("On read");

    QuicServer* server = static_cast<QuicServer*>((void*)stream_ctx);
    if (!server) {
        Logger::getInstance().LOG("Context invalid, aborting stream");
        lsquic_conn_abort(lsquic_stream_conn(stream));
        return;
    }

    unsigned char buf[1];
    int nread = lsquic_stream_read(stream, buf, sizeof(buf));
    if (nread > 0) {
        Logger::getInstance().LOGF("Read %d byte(s)", nread);
        server->recvbuf.append(buf[0]);
        if (buf[0] == '\n') {
            Logger::getInstance().LOG("Read newline");
            lsquic_stream_wantread(stream, 0);
            lsquic_stream_wantwrite(stream, 1);
        }
    } else if (nread == 0) {
        Logger::getInstance().LOG("Read EOF");
        lsquic_stream_shutdown(stream, 0);
        if (!server->recvbuf.isEmpty()) {
            lsquic_stream_wantwrite(stream, 1);
        }
    } else {
        Logger::getInstance().LOGF("error reading from stream (errno: %d) -- abort connection", errno);
        lsquic_conn_abort(lsquic_stream_conn(stream));
    }
}

static void
server_on_write (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("On write");

    QuicServer* server = static_cast<QuicServer*>((void*)stream_ctx);
    if (!server) {
        Logger::getInstance().LOG("Context invalid, aborting stream");
        lsquic_conn_abort(lsquic_stream_conn(stream));
        return;
    }

    const QByteArray& echoData = server->recvbuf;

    int n = lsquic_stream_write(stream, echoData, echoData.size());

    if (n > 0) {
        int remaining = echoData.size() - n;
        if (remaining == 0) {
            Logger::getInstance().LOGF("All %d bytes written to stream", n);
            lsquic_stream_close(stream);
        } else {
            Logger::getInstance().LOGF("Still %d bytes remaining to write", remaining);
        }
    } else {
        /* When `on_write()' is called, the library guarantees that at least
         * something can be written.  If not, that's an error whether 0 or -1
         * is returned.
         */
        Logger::getInstance().LOGF("stream_write() returned %d, abort connection", n);
        lsquic_conn_abort(lsquic_stream_conn(stream));
    }

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

