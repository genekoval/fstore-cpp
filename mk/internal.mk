# Definitions for all internal static libraries

repo = librepo
$(repo).type = static

service = libservice
$(service).type = static
$(service).deps = $(repo)

internal = service repo
internal.libs = $(addprefix lib,$(internal))
