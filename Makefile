.POSIX:

include defaults.mk
-include local.mk

objects = main.o problems/1.o problems/2.o problems/3.o problems/4.o utils/factors.o
deps := $(objects:%.o=%.d)
compilation_db := $(objects:%.o=%.json)

files_all := $(objects) $(deps) $(compilation_db) euler compile_commands.json .compile_commands.json.tmp .generated_files.txt

LDFLAGS_all := $(LDFLAGS)
CFLAGS_all := $(CFLAGS) $(CFLAGS_warnings$(ENABLE_WARNINGS))
CPPFLAGS_all := $(CPPFLAGS_deps) $(CPPFLAGS_testing$(ENABLE_TESTING)) $(CPPFLAGS_asserts$(ENABLE_ASSERTS)) $(CPPFLAGS)

all: remove-old-files .WAIT compile_commands.json euler

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

run: euler
	./euler

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
.generated_files.txt: Makefile
	@for file in $(files_all); do echo "$$file" ; done | sort >$@.tmp
	@touch $@
	rm -fv $$(comm -13 $@.tmp $@)
	@mv $@.tmp $@

$(objects) $(compilation_db): defaults.mk local.mk
local.mk:
	touch $@

-include $(deps)
.PHONY: all clean clean-local.mk install installdirs uninstall remove-old-files
