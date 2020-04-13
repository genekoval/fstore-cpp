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
 yaml-cpp
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

$(obj)/$(service)/settings.o: CXXFLAGS += -DCONFDIR='"$(prefix)/etc"'
