# My Project Euler Solutions

This repository contains my project euler solutions. They may not be efficient, but they were relatively quick to implement.

## Quickstart

```bash
$ cd project-euler
$ make
$ ./euler 3 # Problem 3 solution
6857
$ ./euler # Solution of the lastest solved problem
[...]
```

## Building

The build options are available in `default.mk`. You may overwrite them on the command line or writing them in a `local.mk` file. If you intend to do incremental compilation, this second method is preferred, as it will rebuild all object files when the configuration options change.

You can do an out of tree build with GNU Make using VPATH :

```bash
$ mkdir -p build build/problems build/utils
$ make -C build -f "$PWD"/project-euler/Makefile" -I "$PWD/project-euler" VPATH="$PWD/project-euler"
$ ./build/euler
```
Note that the `local.mk` option file can be created in the build directory, in order to have multiple build configuration at once.

## Unit testing

You may build and run the tests with the following commands.
```bash
$ make ENABLE_TESTING=1 build_tests
$ ./unit_tests
$ # Alternatively, make ENABLE_TESTING=1 check
```
Note that these commands may not rebuild the objects files, which would result in an error. You may force rebuilding by using `make clean` or by adding `ENABLE_TESTING = 1` to `local.mk`

## Adding solutions to a new problem

You may add a new solution by adding the problem name in `problems.txt` and by adding a `.c` file of the same name in the `problems/` directory.
You may read any file in this directory to find an example.

## Adding a unit test

You may add a unit test by adding its name in `tests.txt`, then defining the test in any c file when the macro TESTING is defined.
To see how to write a test, find the example test `test_infrastructure_works_correctly` in `test.c`
