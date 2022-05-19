TEMPLATE = lib

boringssl.target = bssl
boringssl.commands = echo "Building BoringSSL..."; \
                     $$PWD/../scripts/build-boringssl.sh; \
                     echo "Finished building BoringSSL.";
boringssl.depends =

QMAKE_EXTRA_TARGETS += boringssl
PRE_TARGETDEPS += bssl
