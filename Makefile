.POSIX:
all: build

include defaults.mk
-include local.mk

LDFLAGS_all := $(LDFLAGS)
CFLAGS_all := $(CFLAGS) $(CFLAGS_warnings$(ENABLE_WARNINGS))
CPPFLAGS_all := $(CPPFLAGS_deps) $(CPPFLAGS_testing$(ENABLE_TESTING)) $(CPPFLAGS_asserts$(ENABLE_ASSERTS)) $(CPPFLAGS)

objects_lib = utils/divisibility.o utils/unequality.o utils/reductions.o utils/vec.o utils/bigint.o
objects_exe = main.o
objects_test = test.o
objects_problems =

problems.mk: problems.txt
	echo 'objects_problems += \' >$@
	sed 's_\(.*\)_    problems/\1.o \\_' $^ >>$@
	echo >>$@
# my editor thinks the quotes above are unbalanced so here's one
include problems.mk

objects_all := $(objects_lib) $(objects_exe) $(objects_test) $(objects_problems)
deps := $(objects_all:%.o=%.d)
compilation_db := $(objects_all:%.o=%.json)

files_all := $(objects_all) $(deps) $(compilation_db) euler compile_commands.json .compile_commands.json.tmp .generated_files.txt problems.mk problems.h tests_list.h

build: remove-old-files .WAIT generated_sources .WAIT compile_commands.json euler
build_tests: remove-old-files .WAIT generated_sources .WAIT compile_commands.json unit_tests

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

euler: $(objects_exe) $(objects_lib) $(objects_problems)
unit_tests: $(objects_test) $(objects_lib) $(objects_problems)

euler unit_tests:
	$(CC) $(CFLAGS_all) $(LDFLAGS_all) -o $@ $^

run: build
	./euler
check: build_tests
	./unit_tests

generated_sources: problems.h tests_list.h

problems.h: problems.txt
	sed 's/\(.*\)/PROBLEM_DISPATCH(\1)/' $^ >$@

tests_list.h: tests.txt
	sed 's/\(.*\)/TEST_DISPATCH(\1)/' $^ >$@

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

$(objects_all) $(compilation_db): defaults.mk local.mk
local.mk:
	touch $@

-include $(deps)
.PHONY: all build build_tests clean clean-local.mk install installdirs uninstall run check remove-old-files generated_sources
