unix: {

    LIBS += -L$$OUT_PWD -lcrypto
    PRE_TARGETDEPS += $$OUT_PWD/libcrypto.a

    LIBS += -L$$OUT_PWD -lssl
    PRE_TARGETDEPS += $$OUT_PWD/libssl.a

}

win32: {

    LIBS += -L$$OUT_PWD -lcrypto
    PRE_TARGETDEPS += $$OUT_PWD/crypto.lib

    LIBS += -L$$OUT_PWD -lssl
    PRE_TARGETDEPS += $$OUT_PWD/ssl.lib

}
