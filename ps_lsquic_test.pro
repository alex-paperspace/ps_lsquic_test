TEMPLATE = subdirs

SUBDIRS += \
    client \
    bssl \
    lsquiclib \

client.depends += lsquiclib
lsquiclib.depends += bssl
