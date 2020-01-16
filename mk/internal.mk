# Definitions for all internal static libraries

repo = librepo
service = libservice
util = libutil

$(repo).type = static
$(repo).deps = $(util)

$(service).type = static
$(service).deps = $(util) $(repo)

$(util).type = static

internal = repo service util
internal.libs = $(addprefix lib,$(internal))
