unix: {

LIBS += -L$$OUT_PWD/../ps_lsquic -lps_lsquic
PRE_TARGETDEPS += $$OUT_PWD/../ps_lsquic/libps_lsquic.a

}

win32: {

LIBS += -L$$OUT_PWD/../ps_lsquic/debug -lps_lsquic
PRE_TARGETDEPS += $$OUT_PWD/../ps_lsquic/debug/ps_lsquic.lib

}

INCLUDEPATH += $$PWD/../ps_lsquic \
               ../lsquiclib/lsquic/wincompat \

include(../bssl/link_boringssl.pri)
include(../lsquiclib/link_lsquic.pri)


unix: {
LIBS += -lz

INCLUDEPATH += /usr/local/include \
               /opt/homebrew/include

LIBS += -L/usr/local/lib -L/opt/homebrew/lib -levent
}

win32: {
INCLUDEPATH += C:/vcpkg/installed/x64-windows-static/include

LIBS += C:/vcpkg/installed/x64-windows-static/lib/event.lib

LIBS += C:/vcpkg/installed/x64-windows-static/lib/zlib.lib

LIBS += "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/AdvAPI32.lib"

LIBS += "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/iphlpapi.lib"
}
