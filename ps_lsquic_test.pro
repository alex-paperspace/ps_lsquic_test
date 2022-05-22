TEMPLATE = subdirs

SUBDIRS += client
client.depends += \
    ps_lsquic \
    lsquiclib \
    bssl

SUBDIRS += server
server.depends += \
    ps_lsquic \
    lsquiclib \
    bssl

SUBDIRS += bssl #boringssl build wrapper

SUBDIRS += lsquiclib #lsquic build wrapper
lsquiclib.depends += \
    bssl

SUBDIRS += ps_lsquic #packaged utilities for ps_lsquic
ps_lsquic.depends += \
    lsquiclib \
    bssl


