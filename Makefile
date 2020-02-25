project := fstore
version := 0.1.0

include mk/db.mk
include mk/internal.mk

install := $(project)
targets := $(install) $(internal.libs)

extensions := cli

$(project).type = executable
$(project).deps = $(service) $(extensions)
define $(project).libs
 color++
 commline
 cryptopp
 magix
 pqxx
 pq
 uuid
 $(internal)
endef

include $(DEVROOT)/include/mkbuild/base.mk
include $(DEVROOT)/include/mkbuild/cli.mk

$($(repo)): CXXFLAGS += -DPREFIX='"$(prefix)"'

.PHONY: debug
debug:
	gdb -quiet -tui -ex "source .gdb/break" -x .gdb/run $($(project))
