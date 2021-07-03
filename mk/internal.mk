# Definitions for all internal static libraries

conf = libconf
$(conf).type = static

repo = librepo
$(repo).type = static

service = libservice
$(service).type = static
$(service).deps = $(repo)

server = libserver
$(server).type = static
$(server).deps = $(service)

internal = conf server service repo
internal.libs = $(addprefix lib,$(internal))
