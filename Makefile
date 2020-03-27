project := fstore
version := 0.1.0

include mk/db.mk
include mk/internal.mk

test := $(project)-test

install := $(project)
targets := $(install) $(internal.libs) $(test)

extensions := cli

define common
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
 $(common)
endef

$(test).type = executable
$(test).deps = $(project)
define $(test).libs
 gtest
 gtest_main
 $(common)
endef

include $(DEVROOT)/include/mkbuild/base.mk
include $(DEVROOT)/include/mkbuild/cli.mk

$($(repo)): CXXFLAGS += -DPREFIX='"$(prefix)"'

.PHONY: debug
debug:
	gdb -quiet -tui -ex "source .gdb/break" -x .gdb/run $($(project))

.PHONY: test
test: $(test)
	$($(test))
