
LIBS += -L$$OUT_PWD/../bssl/ssl -lssl
PRE_TARGETDEPS += $$OUT_PWD/../bssl/ssl/libssl.a

LIBS += -L$$OUT_PWD/../bssl/crypto -lcrypto
PRE_TARGETDEPS += $$OUT_PWD/../bssl/crypto/libcrypto.a

INCLUDEPATH += $$PWD/../bssl/boringssl/include
