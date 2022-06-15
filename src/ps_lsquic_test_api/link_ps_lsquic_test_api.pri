CONFIG(release, debug|release) {

    unix: {
        LIBS += -L$$OUT_PWD/../ps_lsquic_test_api/release -lps_lsquic
        PRE_TARGETDEPS += $$OUT_PWD/../ps_lsquic_test_api/release/libps_lsquic.a
    }

    win32: {
        LIBS += -L$$OUT_PWD/../ps_lsquic_test_api/release -lps_lsquic
        PRE_TARGETDEPS += $$OUT_PWD/../ps_lsquic_test_api/release/ps_lsquic.lib
    }

}

CONFIG(debug, debug|release) {

    unix: {
        LIBS += -L$$OUT_PWD/../ps_lsquic_test_api/debug -lps_lsquic
        PRE_TARGETDEPS += $$OUT_PWD/../ps_lsquic_test_api/debug/libps_lsquic.a
    }

    win32: {
        LIBS += -L$$OUT_PWD/../ps_lsquic_test_api/debug -lps_lsquic
        PRE_TARGETDEPS += $$OUT_PWD/../ps_lsquic_test_api/debug/ps_lsquic.lib
    }

}

include(link_boringssl.pri)
include(link_lsquic.pri)

#zlib
unix: {
    LIBS += -lz
}

win32: {
    LIBS += -L$$OUT_PWD -lzlib
    PRE_TARGETDEPS += $$OUT_PWD/zlib.lib

    LIBS += "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/AdvAPI32.lib"

    LIBS += "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/iphlpapi.lib"
}
