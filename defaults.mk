CC = gcc

ENABLE_ASSERTS = 0
ENABLE_WARNINGS = 0
ENABLE_TESTING = 0

CFLAGS = -g -O3 -std=c23
CFLAGS_warnings0 =
CFLAGS_warnings1 = -Wall -Wextra

LDFLAGS =

CPPFLAGS = -I.
CPPFLAGS_deps = -MMD -MP
CPPFLAGS_testing0 =
CPPFLAGS_testing1 =
CPPFLAGS_asserts0 = -DNDEBUG
CPPFLAGS_asserts1 =

PREFIX = /usr/local
DESTDIR =
BINDIR = $(PREFIX)/bin
INSTALL = cp
