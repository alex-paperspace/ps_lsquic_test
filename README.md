# LSQUIC in C++

LSQUIC is an open source quic library written in C. This is a testing client/server tool trying to get basic functionality out of the library's low level interface in a C++ lib with a qmake build system.

# Building

Copy correct platform libs to output of either client or server. These libs are located in `./ps_lsquic/out`

# Usage

Very unpolished at the moment. For the server, you have to generate your own openssl cert and key.

Once that is done, place the cert and key where the server binary is located or above it, and rename them to cert.pem and key.pem.

Once, the engine is instantiated on both sides with no errors, you should be able to communicate once you listen and connect.

# Status 

Since this project is not finished, here's what I'm working on:

- ~~Get the handshake to function. Since these are clients/servers run on my local network, sni parsing doesn't exactly work as expected yet.~~
- ~~provide sni input on client~~
- ~~start working on client/server callbacks~~
- Improve stability, especially for cross-platform communication

