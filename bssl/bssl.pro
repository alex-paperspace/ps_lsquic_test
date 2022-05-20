TEMPLATE = aux

boringssl.commands =    export SOURCEDIR=$$PWD/..; \
                        export LC_ALL=C; \
                        $$PWD/../scripts/build-boringssl.sh; \


QMAKE_EXTRA_TARGETS += boringssl
PRE_TARGETDEPS += boringssl
