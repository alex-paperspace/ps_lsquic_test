TEMPLATE = aux

unix: {
boringssl.commands =    export BORINGSSL_SRC=$$PWD/boringssl; \
                        $$PWD/../scripts/build-boringssl.sh; \
}

win32: {
boringssl.commands =    set BORINGSSL_SRC=$$PWD/boringssl; \
                        @echo %BORINGSSL_SRC%; \
                        start  "$$PWD/../scripts/build-boringssl-windows.bat" \
}

QMAKE_EXTRA_TARGETS += boringssl
PRE_TARGETDEPS += boringssl

