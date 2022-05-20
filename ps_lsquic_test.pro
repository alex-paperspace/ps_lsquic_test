TEMPLATE = subdirs

SUBDIRS += \
    bssl \
    client \
    lsquiclib

client.depends += lsquic
lsquiclib.depends += bssl
