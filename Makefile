project := fstore
summary := Simple object storage.

STD := c++20

include mk/internal.mk

client := lib$(project)

install := $(project) $(client)
targets := $(install) $(internal.libs)

define core.libs
 $(internal)
 conftools
 cryptopp
 entix
 ext++
 fmt
 magix
 netcore
 pqxx
 pq
 timber
 uuid++
 yaml-cpp
endef

test.deps = $(internal.libs)
define test.libs
 $(core.libs)
 $(project)
 gtest
endef

$(project).type = executable
$(project).deps = $(internal.libs)
define $(project).libs
 $(core.libs)
 commline
 dmon
endef

$(client).type = shared
define $(client).libs
 netcore
endef

include mkbuild/base.mk

confdir = $(prefix)/etc/$(project)

$(obj)/$(core)/settings.o: CXXFLAGS += -DCONFDIR='"$(prefix)/etc"'

$(obj)/$(project)/main.o: CXXFLAGS +=\
 -DNAME='"$(project)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(summary)"'\
 -DCONFDIR='"$(confdir)"'

$(obj)/$(project)/db/db.o: CXXFLAGS +=\
 -DSQLDIR='"$(shell pwd)/db"'

edit.config:
	$(EDITOR) $(confdir)/fstore.yml

start:
	$($(project)) start
