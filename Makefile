project := fstore
summary := Simple object storage.

STD := c++20

include mk/internal.mk

client := lib$(project)

install := $(project) $(client)
targets := $(install) $(internal.libs)

install.directories := \
 include/fstore \
 share/fstore

core.libs := \
 $(internal) \
 $(project) \
 conftools \
 crypto \
 cryptopp \
 ext++ \
 fmt \
 magix \
 netcore \
 pg++ \
 pthread \
 ssl \
 timber \
 uuid++ \
 yaml-cpp \
 zipline

test.deps = $(internal.libs)
test.libs := \
 $(core.libs) \
 gtest \
 gmock \
 pthread

$(project).type = executable
$(project).deps = $(internal.libs) $(client)
$(project).libs := \
 $(core.libs) \
 commline \
 dbtools \
 dmon

$(client).type = shared
$(client).libs := \
 netcore \
 uuid++

files = \
 $(include) \
 $(src) \
 mk \
 share \
 Makefile \
 VERSION

include mkbuild/base.mk

defines.develop = TEST TIMBER_TIMER_ACTIVE

confdir ?= $(prefix)/etc/$(project)

$(obj)/$(project)/main.o: CXXFLAGS +=\
 -DNAME='"$(project)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(summary)"'\
 -DCONFDIR='"$(confdir)"'

edit.config:
	$(EDITOR) $(confdir)/fstore.yml

start:
	$($(project)) start
