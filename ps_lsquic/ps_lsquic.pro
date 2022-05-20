TEMPLATE = lib
CONFIG += staticlib
TARGET = ps_lsquic

QT       += core

CONFIG += c++17

INCLUDEPATH += \
        client \
        common \
        server \

HEADERS += \
        client/clientcallbacks.h \
        client/ps_lsquicclient.h \
        common/logger.h \
        common/ps_lsquic_lib_initializer.h \
        common/ps_lsquic_ssl.h \
        common/ps_lsquicendpoint.h \
        common/ps_lsquicengine.h \
        server/ps_lsquicserver.h \
        server/servercallbacks.h \


SOURCES += \
        client/clientcallbacks.cpp \
        client/ps_lsquicclient.cpp \
        common/logger.cpp \
        common/ps_lsquic_lib_initializer.cpp \
        common/ps_lsquic_ssl.cpp \
        common/ps_lsquicendpoint.cpp \
        common/ps_lsquicengine.cpp \
        server/ps_lsquicserver.cpp \
        server/servercallbacks.cpp \

include(../bssl/link_boringssl.pri)
include(../lsquiclib/link_lsquic.pri)

#zlib
LIBS += -lz
