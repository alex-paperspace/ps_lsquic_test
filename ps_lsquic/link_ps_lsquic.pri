
LIBS += -L$$OUT_PWD/../ps_lsquic -lps_lsquic
PRE_TARGETDEPS += $$OUT_PWD/../ps_lsquic/libps_lsquic.a

INCLUDEPATH += $$PWD/../ps_lsquic

include(../bssl/link_boringssl.pri)
include(../lsquiclib/link_lsquic.pri)

#zlib
LIBS += -lz

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -levent
