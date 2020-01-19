# Definitions for all internal static libraries

repo = librepo
core = libcore

$(repo).type = static

$(core).type = static
$(core).deps = $(repo)

internal = repo core
internal.libs = $(addprefix lib,$(internal))
