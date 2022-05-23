unix: {

LIBS += -L$$OUT_PWD/../bssl/ssl -lssl
PRE_TARGETDEPS += $$OUT_PWD/../bssl/ssl/libssl.a

LIBS += -L$$OUT_PWD/../bssl/crypto -lcrypto
PRE_TARGETDEPS += $$OUT_PWD/../bssl/crypto/libcrypto.a

}

win32: {

LIBS += -L$$OUT_PWD/../bssl/ssl/Debug -lssl
PRE_TARGETDEPS += $$OUT_PWD/../bssl/ssl/Debug/ssl.lib

LIBS += -L$$OUT_PWD/../bssl/crypto/Debug -lcrypto
PRE_TARGETDEPS += $$OUT_PWD/../bssl/crypto/Debug/crypto.lib

}

INCLUDEPATH += $$PWD/../bssl/boringssl/include
