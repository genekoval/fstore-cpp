# mkbuild {{{
project := fstore
version := 0.1.0

install := $(project)
targets := $(install)

extensions := cli

$(project).type = executable
define $(project).libs
 color++
 commline
 pqxx
 pq
 uuid
endef
$(project).deps = cli

include $(DEVROOT)/include/mkbuild/base.mk
include $(DEVROOT)/include/mkbuild/cli.mk
# }}}

# sql {{{
sql := db
sql.schemata := $(sql)/schemata
sql.schemata.files := $(wildcard $(sql.schemata)/*.sql)
sql.schemata.bases := $(subst $(sql.schemata)/,,$(sql.schemata.files))
sql.schemata.items := $(subst .sql,,$(sql.schemata.bases))

db.client := psql
db.project := $(db.client) --username=$(project) --dbname=$(project)
db.init := $(sql)/init.sql
define db.run
 $(db.project) --file=$(1)
endef

define sql.schemata.item
 schema.$(1):
	$(call db.run,$(sql.schemata)/$(1).sql)
endef

$(foreach item,$(sql.schemata.items),$(eval $(call sql.schemata.item,$(item))))

.PHONY: db schemata

db:
	$(db.project)

db.init:
	$(db.client) --username=DB_SUPERUSER --file=$(db.init)

schemata: $(addprefix schema.,$(sql.schemata.items))
# }}}
