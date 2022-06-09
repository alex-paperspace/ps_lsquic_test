#include "clientcallbacks.h"
#include "ps_lsquicclient.h"
#include "common/logger.h"

namespace paperspace {
namespace lsquic {

static lsquic_conn_ctx_t *
client_on_new_conn (void *stream_if_ctx, struct lsquic_conn *conn)
{
    Logger::getInstance().LOG("New connection");
    return static_cast<lsquic_conn_ctx_t*>(stream_if_ctx);
}

static void
client_on_conn_closed (struct lsquic_conn *conn)
{
    Logger::getInstance().LOG("Connection closed");
    lsquic_conn_ctx_t* ctx = lsquic_conn_get_ctx(conn);
    QuicClient* client = static_cast<QuicClient*>((void*)ctx);
    if (!client) {
        Logger::getInstance().LOG("Context Invalid, aborting conn closure");
        return;
    }

    client->cleanup();

}

static lsquic_stream_ctx_t *
client_on_new_stream (void *stream_if_ctx, struct lsquic_stream *stream)
{
    Logger::getInstance().LOG("New stream");
    if (!stream) {
        Logger::getInstance().LOG("Stream is null, conn going away");
        return nullptr;
    }

    QuicClient* client = static_cast<QuicClient*>((void*)stream_if_ctx);
    if (!client) {
        Logger::getInstance().LOG("Context invalid, aborting stream");
        lsquic_conn_abort(lsquic_stream_conn(stream));
        return nullptr;
    }

    client->recvbuf.clear();
    lsquic_stream_wantwrite(stream, 1);
    return static_cast<lsquic_stream_ctx_t*>(stream_if_ctx);
}

static void
client_on_read (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("On read");

    QuicClient* client = static_cast<QuicClient*>((void*)stream_ctx);
    if (!client) {
        Logger::getInstance().LOG("Context Invalid, aborting read");
        return;
    }
    unsigned char buf[1];
    int nread = lsquic_stream_read(stream, buf, sizeof(buf));
    if (nread > 0) {
        Logger::getInstance().LOGF("Read %d byte(s)", nread);
        client->recvbuf.append(buf[0]);
    } else if (nread == 0) {
        Logger::getInstance().LOG("Read EOF");
        emit client->receivedDataSignal(client->recvbuf);
        lsquic_stream_shutdown(stream, 0);
    } else {
        Logger::getInstance().LOGF("error reading from stream (errno: %d) -- abort connection", errno);
        lsquic_conn_abort(lsquic_stream_conn(stream));
    }
}

static void
client_on_write (struct lsquic_stream *stream, lsquic_stream_ctx_t *stream_ctx)
{
    Logger::getInstance().LOG("On write");

    QuicClient* client = static_cast<QuicClient*>((void*)stream_ctx);
    if (!client) {
        Logger::getInstance().LOG("Context Invalid, aborting write");
        return;
    }
    const QByteArray& buf = client->dataToSend;

    int n = lsquic_stream_write(stream, buf, buf.size());

    if (n > 0) {
        int remaining = buf.size() - n;
        if (remaining == 0) {
            Logger::getInstance().LOGF("All %d bytes written to stream", n);
            lsquic_stream_shutdown(stream, 1);
            lsquic_stream_wantread(stream, 1);
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

