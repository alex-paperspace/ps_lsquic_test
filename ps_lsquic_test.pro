TEMPLATE = subdirs

SUBDIRS += client
client.subdir = src/client
client.depends += \
    ps_lsquic_test_api \


SUBDIRS += server
server.subdir = src/server
server.depends += \
    ps_lsquic_test_api \


SUBDIRS += ps_lsquic_test_api
ps_lsquic_test_api.subdir = src/ps_lsquic_test_api
