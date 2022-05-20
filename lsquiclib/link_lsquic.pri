
LIBS += -L$$OUT_PWD/../lsquiclib/src/liblsquic -llsquic
PRE_TARGETDEPS += $$OUT_PWD/../lsquiclib/src/liblsquic/liblsquic.a

INCLUDEPATH += $$PWD/../lsquiclib/lsquic/include
