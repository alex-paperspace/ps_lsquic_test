
LIBS += -L$$OUT_PWD/../ps_lsquic -lps_lsquic
PRE_TARGETDEPS += $$OUT_PWD/../ps_lsquic/libps_lsquic.a

INCLUDEPATH += $$PWD/../ps_lsquic

include(../bssl/link_boringssl.pri)
include(../lsquiclib/link_lsquic.pri)

#zlib
LIBS += -lz

unix: {
INCLUDEPATH += /usr/local/include \
               /opt/homebrew/include \
LIBS += -L/usr/local/lib -L/opt/homebrew/lib -levent
}
