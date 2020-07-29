project := fstore
summary := Simple object storage.

STD := gnu++2a

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
 netcore
 timber
endef

BUILD = /tmp/$(project)

include mk/db.mk
include mkbuild/base.mk

$(obj)/$(service)/settings.o: CXXFLAGS += -DCONFDIR='"$(prefix)/etc"'

$(obj)/$(project)/main.o: CXXFLAGS +=\
 -DNAME='"$(project)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(summary)"'
