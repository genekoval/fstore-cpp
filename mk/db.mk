sql := db
sql.schemata := $(sql)/schemata
sql.schemata.files := $(wildcard $(sql.schemata)/*.sql)
sql.schemata.bases := $(subst $(sql.schemata)/,,$(sql.schemata.files))
sql.schemata.items := $(subst .sql,,$(sql.schemata.bases))

db.username := $(project)
db.dbname := $(project)

db.connect := psql --username=$(db.username) --dbname=$(db.dbname)
db.command := $(db.connect) --quiet --command

define db.exec
 $(db.connect) --file=$(1)
endef

db.superuser ?= postgres

define sql.schemata.item
 schema.$(1):
	$(call db.exec,$(sql.schemata)/$(1).sql)
endef

$(foreach item,$(sql.schemata.items),$(eval $(call sql.schemata.item,$(item))))

.PHONY: db

db:
	@$(db.connect) --quiet

db.create:
	@echo Initializing database...
	@createuser --username=$(db.superuser) $(db.username)
	@createdb --username=$(db.superuser) --owner=$(db.username) $(db.dbname)
	@$(db.command) "ALTER DATABASE $(db.dbname) SET search_path = storage"
	@echo Created database: $(db.dbname)

db.drop:
	@echo Dropping database...
	@dropdb --username=$(db.superuser) --if-exists $(db.dbname)
	@dropuser --username=$(db.superuser) --if-exists $(db.username)
	@echo Removed database: $(db.dbname)

db.schema: $(addprefix schema.,$(sql.schemata.items))
