TEMPLATE = aux

lsquic.commands =    export LSQUIC_SRC=$$PWD/lsquic; \
                     export BORINGSSL_BUILD=$$OUT_PWD/../bssl; \
                     $$PWD/../scripts/build-lsquic.sh; \

QMAKE_EXTRA_TARGETS += lsquic
PRE_TARGETDEPS += lsquic
