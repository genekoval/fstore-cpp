# Definitions for all internal static libraries

conf = libconf
$(conf).type = static

core = libcore
$(core).type = static

server = libserver
$(server).type = static
$(server).deps = $(core)

internal = conf server core
internal.libs = $(addprefix lib,$(internal))
