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

generated_headers = problems.h tests_list.h

problems.mk: problems.txt
	echo 'objects_problems += \' >$@
	sed -e '/^#/d' -e 's_\(.*\)_    problems/\1.o \\_' $^ >>$@
	echo >>$@
include problems.mk

objects_all := $(objects_lib) $(objects_exe) $(objects_test) $(objects_problems)
deps := $(objects_all:%.o=%.d)
compilation_db := $(objects_all:%.o=%.json)
test_lists := $(objects_all:%.o=%.tests)

files_to_clean := $(objects_all) $(deps) $(compilation_db) $(test_lists) $(generated_headers) euler unit_tests compile_commands.json .compile_commands.json.tmp .generated_files.txt problems.mk tests.txt lib.a problems.a

build: dev .WAIT euler
build_tests: dev .WAIT unit_tests

dev: remove-old-files .WAIT $(generated_headers) compile_commands.json

clean: remove-old-files clean-local.mk
	rm -fv $(files_to_clean)
clean-local.mk:
	[ ! -f local.mk -o -s local.mk ] || rm -fv local.mk

install: installdirs
	$(INSTALL) euler $(DESTDIR)$(BINDIR)/euler

installdirs:
	mkdir -p $(DESTDIR)$(BINDIR)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/euler

euler: $(objects_exe) lib.a problems.a
unit_tests: $(objects_test) lib.a problems.a
euler unit_tests:
	$(CC) $(CFLAGS_all) $(LDFLAGS_all) -o $@ $^

lib.a: lib.a($(objects_lib))
problems.a: problems.a($(objects_problems))
lib.a problems.a:
	$(AR) $(ARFLAGS) $@ $?

run: build
	./euler
check: build_tests tests.txt
	for t in $$(cat tests.txt); do ./unit_tests --test=$$t || exit $$?; done

problems.h: problems.txt
	sed -e '/^#/d' -e 's/\(.*\)/PROBLEM_DISPATCH(\1)/' $^ >$@
tests_list.h: tests.txt
	sed -e '/^#/d' -e 's/\(.*\)/TEST_DISPATCH(\1)/' $^ >$@

.SUFFIXES:
.SUFFIXES: .c .a .o .json .tests

.c.o:
	$(CC) $(CFLAGS_all) $(CPPFLAGS_all) -c -o $@ $<
.o.a: ;

tests.txt: $(test_lists)
	sort -m $^ >$@
.c.tests:
	sed -e '/^[ \t]*TEST(\(.*\))$$/!d' -e 's/^[ \t]*TEST(\(.*\))$$/\1/' $< | sort >$@

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
	@for file in $(files_to_clean); do echo "$$file" ; done | sort >$@.tmp
	@touch $@
	rm -fv $$(comm -13 $@.tmp $@)
	@mv $@.tmp $@

$(objects_all) $(compilation_db): defaults.mk local.mk
local.mk:
	touch $@

-include $(deps)
.PHONY: all build build_tests dev clean clean-local.mk install installdirs uninstall run check remove-old-files
