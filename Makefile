project := fstore
summary := Simple object storage.

STD := gnu++2a

include mk/internal.mk

client := lib$(project)

install := $(project) $(client)
targets := $(install) $(internal.libs)

define core.libs
 $(internal)
 conftools
 cryptopp
 ext++
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

BUILD = /tmp/$(project)

include mk/db.mk
include mkbuild/base.mk

confdir = $(prefix)/etc/$(project)

$(obj)/$(service)/settings.o: CXXFLAGS += -DCONFDIR='"$(prefix)/etc"'

$(obj)/$(project)/main.o: CXXFLAGS +=\
 -DNAME='"$(project)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(summary)"'\
 -DCONFDIR='"$(confdir)"'
