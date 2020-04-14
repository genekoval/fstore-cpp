project := fstore
version := 0.1.0

include mk/internal.mk

install := $(project)
targets := $(install) $(internal.libs)

extensions := cli

define core.libs
 cryptopp
 ext++
 magix
 pqxx
 pq
 uuid++
 yaml-cpp
 $(internal)
endef

define test.libs
 $(core.libs)
 gtest
 gtest_main
endef

$(project).type = executable
$(project).deps = $(service) $(extensions)
define $(project).libs
 $(core.libs)
 commline
endef

BUILD = /tmp/$(project)

include mk/db.mk
include $(DEVROOT)/include/mkbuild/base.mk
include $(DEVROOT)/include/mkbuild/cli.mk

$(obj)/$(service)/settings.o: CXXFLAGS += -DCONFDIR='"$(prefix)/etc"'
