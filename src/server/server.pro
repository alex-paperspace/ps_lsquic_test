QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += WIN32_LEAN_AND_MEAN

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

PS_LSQUIC_TEST_API_DIR = $$PWD/../ps_lsquic_test_api
PS_LSQUIC_DIR = $$PWD/../../ps_lsquic

INCLUDEPATH += \
    $$PS_LSQUIC_TEST_API_DIR/client \
    $$PS_LSQUIC_TEST_API_DIR/common \
    $$PS_LSQUIC_TEST_API_DIR/server \
    $$PS_LSQUIC_DIR/boringssl/include \
    $$PS_LSQUIC_DIR/lsquic/include \

win32: {
    INCLUDEPATH += $$PS_LSQUIC_DIR/lsquic/wincompat
}

include($$PS_LSQUIC_TEST_API_DIR/link_ps_lsquic_test_api.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
