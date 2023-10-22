CC=gcc
CFLAGS=-g -Wall -Wpedantic -Wextra -Werror -std=c2x
TESTFLAGS=${CFLAGS} 
BUILDFLAGS=${CFLAGS} -fPIC

tests/test_%: tests/%.o tests/%.test.o tests
	${CC} ${TESTFLAGS} -c $^ -o $@

tests/test_filter: tests/filter.test.o tests/filter.o tests/savgol.o tests/buffer.o  tests/window.o ext/munit/munit.o tests
	${CC} ${TESTFLAGS} tests/filter.test.o tests/filter.o tests/buffer.o tests/savgol.o tests/window.o ext/munit/munit.o -lm -o $@

tests/test_pll: tests/pll.test.o tests/pll.o tests/buffer.o tests/sinusoid.o tests/window.o ext/munit/munit.o tests
	${CC} ${TESTFLAGS} tests/pll.test.o tests/pll.o tests/filter.o tests/buffer.o tests/savgol.o tests/sinusoid.o tests/window.o ext/munit/munit.o -lm -o $@

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