# Definitions for all internal static libraries

core = libcore
$(core).type = static

repo = librepo
$(repo).type = static
$(repo).deps = $(core)

service = libservice
$(service).type = static
$(service).deps = $(repo)

internal = service repo core
internal.libs = $(addprefix lib,$(internal))
