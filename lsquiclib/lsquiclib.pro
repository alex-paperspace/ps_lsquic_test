TEMPLATE = aux

lsquic.commands =    export SOURCEDIR=$$PWD/..; \
                     export LC_ALL=C; \
                     $$PWD/../scripts/build-lsquic.sh; \


QMAKE_EXTRA_TARGETS += lsquic
PRE_TARGETDEPS += lsquic
