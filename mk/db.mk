db ?= psql

createdb ?= createdb
createuser ?= createuser

dropdb ?= dropdb
dropuser ?= dropuser

schemata := db
schemata.files := $(wildcard $(schemata)/*.sql)
schemata.bases := $(subst $(schemata)/,,$(schemata.files))
schemata.items := $(subst .sql,,$(schemata.bases))

db.superuser ?= postgres
db.username := $(project)
db.dbname := $(project)

db.connect := $(db) --username=$(db.username) --dbname=$(db.dbname)
db.command := $(db.connect) --quiet --command

define db.exec
 $(db.connect) --file=$(1)
endef

define schemata.item
 schema.$(1):
	$(call db.exec,$(schemata)/$(1).sql)
endef

$(foreach item,$(schemata.items),$(eval $(call schemata.item,$(item))))

.PHONY: db
db:
	@$(db.connect) --quiet

db.create:
	@echo Initializing database...
	@$(createuser) --username=$(db.superuser) $(db.username)
	@$(createdb) --username=$(db.superuser) --owner=$(db.username) $(db.dbname)
	@$(db.command) "ALTER DATABASE $(db.dbname) SET search_path = storage"
	@echo Created database: $(db.dbname)

db.drop:
	@echo Dropping database...
	@$(dropdb) --username=$(db.superuser) --if-exists $(db.dbname)
	@$(dropuser) --username=$(db.superuser) --if-exists $(db.username)
	@echo Removed database: $(db.dbname)

db.schema: $(addprefix schema.,$(schemata.items))
