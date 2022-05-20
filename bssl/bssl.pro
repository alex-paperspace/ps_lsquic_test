TEMPLATE = aux

boringssl.commands =    export BORINGSSL_SRC=$$PWD/boringssl; \
                        $$PWD/../scripts/build-boringssl.sh; \

QMAKE_EXTRA_TARGETS += boringssl
PRE_TARGETDEPS += boringssl
