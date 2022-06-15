TEMPLATE = lib
CONFIG += staticlib
TARGET = ps_lsquic

QT       += core network

CONFIG += c++17

DEFINES += WIN32_LEAN_AND_MEAN

INCLUDEPATH += \
        ../../ps_lsquic/boringssl/include \
        ../../ps_lsquic/lsquic/include \
        ../../ps_lsquic/lsquic/wincompat \
        client \
        common \
        server

HEADERS += \
        client/clientcallbacks.h \
        client/ps_lsquicclient.h \
        common/logger.h \
        common/ps_lsquic_lib_initializer.h \
        common/ps_lsquic_ssl.h \
        common/ps_lsquicendpoint.h \
        common/ps_lsquicengine.h \
        common/ps_lsquicutil.h \
        server/ps_lsquicserver.h \
        server/servercallbacks.h


SOURCES += \
        client/clientcallbacks.cpp \
        client/ps_lsquicclient.cpp \
        common/logger.cpp \
        common/ps_lsquic_lib_initializer.cpp \
        common/ps_lsquic_ssl.cpp \
        common/ps_lsquicendpoint.cpp \
        common/ps_lsquicengine.cpp \
        common/ps_lsquicutil.cpp \
        server/ps_lsquicserver.cpp \
        server/servercallbacks.cpp

#zlib
LIBS += -lz

