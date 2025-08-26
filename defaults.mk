CC = gcc
MAKEDEP = $(CC) $(CFLAGS_all) $(CPPFLAGS_all) -MM -MP -MF $@ $<

ENABLE_ASSERTS = 0
ENABLE_WARNINGS = 0
ENABLE_TESTING = 0

CFLAGS = -g -O3 -std=c23
CFLAGS_warnings0 =
CFLAGS_warnings1 = -Wall -Wextra

LDFLAGS =

CPPFLAGS = -I.
CPPFLAGS_testing0 =
CPPFLAGS_testing1 = -DTESTING
CPPFLAGS_asserts0 = -DNDEBUG
CPPFLAGS_asserts1 =

PREFIX = /usr/local
DESTDIR =
BINDIR = $(PREFIX)/bin
INSTALL = cp
