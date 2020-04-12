project := fstore
version := 0.1.0

include mk/internal.mk

install := $(project)
targets := $(install) $(internal.libs)

extensions := cli

define test.libs
 color++
 cryptopp
 extensions++
 magix
 pqxx
 pq
 uuid++
 $(internal)
endef

$(project).type = executable
$(project).deps = $(service) $(extensions)
define $(project).libs
 commline
 $(test.libs)
endef

BUILD = /tmp/$(project)

include mk/db.mk
include $(DEVROOT)/include/mkbuild/base.mk
include $(DEVROOT)/include/mkbuild/cli.mk

.PHONY: debug
debug:
	gdb -quiet -tui -ex "source .gdb/break" -x .gdb/run $($(project))
