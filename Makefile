project := fstore
summary := Simple object storage.

STD := c++20

include mk/internal.mk

client := lib$(project)

install := $(project) $(client)
targets := $(install) $(internal.libs)

install.directories := include/fstore share/fstore

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
 pthread
 threadpool
 timber
 uuid++
 yaml-cpp
endef

test.deps = $(internal.libs)
define test.libs
 $(core.libs)
 $(project)
 gtest
 gmock
 pthread
endef

$(project).type = executable
$(project).deps = $(internal.libs)
define $(project).libs
 $(core.libs)
 commline
 dbtools
 dmon
endef

$(client).type = shared
define $(client).libs
 netcore
 uuid++
endef

include mkbuild/base.mk

ifeq ($(environment),$(environment.develop))
 CXXFLAGS += -DTEST
endif

confdir = $(prefix)/etc/$(project)

$(obj)/$(project)/main.o: CXXFLAGS +=\
 -DNAME='"$(project)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(summary)"'\
 -DCONFDIR='"$(confdir)"'

edit.config:
	$(EDITOR) $(confdir)/fstore.yml

start:
	$($(project)) start
