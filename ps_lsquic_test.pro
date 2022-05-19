TEMPLATE = subdirs

SUBDIRS += \
    bssl \
    client

client.depends += bssl
