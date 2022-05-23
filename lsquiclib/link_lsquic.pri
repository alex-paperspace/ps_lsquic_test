
unix: {

LIBS += -L$$OUT_PWD/../lsquiclib/src/liblsquic -llsquic
PRE_TARGETDEPS += $$OUT_PWD/../lsquiclib/src/liblsquic/liblsquic.a

}

win32: {

LIBS += -L$$OUT_PWD/../lsquiclib/src/liblsquic/Debug -llsquic
PRE_TARGETDEPS += $$OUT_PWD/../lsquiclib/src/liblsquic/Debug/lsquic.lib

}

INCLUDEPATH += $$PWD/../lsquiclib/lsquic/include
