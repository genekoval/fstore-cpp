# Definitions for all internal static libraries

repo = librepo
$(repo).type = static
$(repo).deps = $(core)

service = libservice
$(service).type = static
$(service).deps = $(repo)

internal = service repo
internal.libs = $(addprefix lib,$(internal))
