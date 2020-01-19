include mk/db.mk
include mk/internal.mk

project := fstore
version := 0.1.0

install := $(project)
targets := $(install) $(internal.libs)

extensions := cli

$(project).type = executable
$(project).deps = cli $(service)
define $(project).libs
 $(internal)
 color++
 commline
 cryptopp
 pqxx
 pq
 uuid
endef

include $(DEVROOT)/include/mkbuild/base.mk
include $(DEVROOT)/include/mkbuild/cli.mk
