.POSIX:
all:

include defaults.mk
-include local.mk

objects = main.o utils/factors.o utils/unequality.o

problems.mk: problems.txt
	echo 'objects += \' >$@
	sed 's_\(.*\)_    problems/\1.o \\_' $^ >>$@
	echo >>$@
# my editor thinks the quotes above are unbalanced so here's one
include problems.mk

deps := $(objects:%.o=%.d)
compilation_db := $(objects:%.o=%.json)

files_all := $(objects) $(deps) $(compilation_db) euler compile_commands.json .compile_commands.json.tmp .generated_files.txt problems.mk problem_func_names.txt problem_func_declarations.h

LDFLAGS_all := $(LDFLAGS)
CFLAGS_all := $(CFLAGS) $(CFLAGS_warnings$(ENABLE_WARNINGS))
CPPFLAGS_all := $(CPPFLAGS_deps) $(CPPFLAGS_testing$(ENABLE_TESTING)) $(CPPFLAGS_asserts$(ENABLE_ASSERTS)) $(CPPFLAGS)

all: build
build: remove-old-files .WAIT generated_sources .WAIT compile_commands.json euler

clean: remove-old-files clean-local.mk
	rm -fv $(files_all)
clean-local.mk:
	[ ! -f local.mk -o -s local.mk ] || rm -fv local.mk

install: installdirs
	$(INSTALL) output $(DESTDIR)$(BINDIR)/output

installdirs:
	mkdir -p $(DESTDIR)$(BINDIR)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/output

euler: $(objects)
	$(CC) $(CFLAGS_all) $(LDFLAGS_all) -o $@ $^

run: build
	./euler

generated_sources: problem_func_names.txt problem_func_declarations.h

problem_func_names.txt: problems.txt
	sed 's/\(.*\)/problem\1,/' $^ >$@
problem_func_declarations.h: problems.txt
	sed 's/\(.*\)/int64_t problem\1();/' $^ >$@

.SUFFIXES:
.SUFFIXES: .c .o .json

.c.o:
	$(CC) $(CFLAGS_all) $(CPPFLAGS_all) -c -o $@ $<

compile_commands.json: .compile_commands.json.tmp
	@echo '[' >$@
	@sed '$$s/,//' $^ >>$@
	@echo ']' >>$@
.compile_commands.json.tmp: $(compilation_db)
	@cat $^ >$@
.c.json:
	@echo '  {' >$@
	@echo '    "directory": "'"$$PWD"'",' >>$@
	@echo '    "file": "$<",' >>$@
	@echo '    "command": "'$(CC) $(CFLAGS_all) $(CPPFLAGS_all) '-c -o $*.o $<",' >>$@
	@echo '    "output": "$*.o"' >>$@
	@echo '  },' >>$@

remove-old-files: .generated_files.txt
.generated_files.txt: Makefile problems.mk
	@for file in $(files_all); do echo "$$file" ; done | sort >$@.tmp
	@touch $@
	rm -fv $$(comm -13 $@.tmp $@)
	@mv $@.tmp $@

$(objects) $(compilation_db): defaults.mk local.mk
local.mk:
	touch $@

-include $(deps)
.PHONY: all build clean clean-local.mk install installdirs uninstall remove-old-files generated_sources
