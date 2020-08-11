# Definitions for all internal static libraries

repo = librepo
$(repo).type = static

service = libservice
$(service).type = static
$(service).deps = $(repo)

server = libserver
$(server).type = static
$(server).deps = $(service)

internal = server service repo
internal.libs = $(addprefix lib,$(internal))
