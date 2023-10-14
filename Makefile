CC=gcc
CFLAGS=-g -Wall -Wpedantic -Wextra
TESTFLAGS=${CFLAGS} -DSQLITE_CORE 
BUILDFLAGS=${CFLAGS} -fPIC

libsqldsp.so: lib/main.o lib/filter.o lib/functions.o lib/buffer.o lib/savgol.o
	${CC} ${BUILDFLAGS} -shared lib/functions.o lib/buffer.o lib/filter.o lib/savgol.o lib/main.o -o libsqldsp.so

tests/test_%: tests/%.o tests/%.test.o tests
	${CC} ${TESTFLAGS} -c $^ -lsqlite3 -o $@

tests/test_filter: tests/filter.test.o tests/filter.o tests/savgol.o tests/buffer.o ext/munit/munit.o tests
	${CC} ${TESTFLAGS} tests/filter.test.o tests/filter.o tests/buffer.o tests/savgol.o ext/munit/munit.o -lsqlite3 -lm -o $@

tests/test_pll: tests/pll.test.o tests/pll.o tests/buffer.o ext/munit/munit.o tests
	${CC} ${TESTFLAGS} tests/pll.test.o tests/pll.o tests/filter.o tests/buffer.o tests/savgol.o ext/munit/munit.o -lsqlite3 -lm -o $@

tests/%.o: %.c
	${CC} ${TESTFLAGS} -c $< -o $@

tests/%.test.o: tests/%.test.c ext/munit/munit.h
	${CC} ${TESTFLAGS} -c $< -o $@

tests/%.test.c: %.h

lib/%.o: %.c
	${CC} ${BUILDFLAGS} -c $< -o $@

%.c: %.h

ext/munit/munit.o: ext/munit/munit.c ext/munit/munit.h
	${CC} -c $< -o $@

tests:
	mkdir -p tests

test: tests tests/test_filter tests/test_pll

clean:
	rm -rf tests/*.o
	rm -rf bin/*
	rm -rf lib/*
	rm -f *.so

.PHONY: clean tests test