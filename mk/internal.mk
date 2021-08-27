# Definitions for all internal static libraries

conf = libconf
$(conf).type = static

repo = librepo
$(repo).type = static

core = libcore
$(core).type = static
$(core).deps = $(repo)

server = libserver
$(server).type = static
$(server).deps = $(core)

internal = conf core repo server
internal.libs = $(addprefix lib,$(internal))
