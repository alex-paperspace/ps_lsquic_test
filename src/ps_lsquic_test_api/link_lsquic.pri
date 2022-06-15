unix: {

    LIBS += -L$$OUT_PWD -llsquic
    PRE_TARGETDEPS += $$OUT_PWD/liblsquic.a

}

win32: {

    LIBS += -L$$OUT_PWD -llsquic
    PRE_TARGETDEPS += $$OUT_PWD/lsquic.lib

}
