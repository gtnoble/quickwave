CC=gcc
CFLAGS=-g -Wall -Wpedantic -Wextra -Werror -std=c2x
TESTFLAGS=${CFLAGS} 
BUILDFLAGS=${CFLAGS} -fPIC

tests/test_%: tests/%.o tests/%.test.o tests
	${CC} ${TESTFLAGS} -c $^ -o $@

tests/test_buffer: tests/buffer.test.o tests/buffer.o ext/munit/munit.o
	${CC} ${TESTFLAGS} $^ -lm -o $@

tests/test_filter: tests/filter.test.o tests/filter.o tests/savgol.o tests/buffer.o  tests/window.o ext/munit/munit.o
	${CC} ${TESTFLAGS} $^ -lm -o $@

tests/test_pll: tests/pll.test.o tests/filter.o tests/pll.o tests/buffer.o tests/sinusoid.o tests/window.o tests/savgol.o ext/munit/munit.o
	${CC} ${TESTFLAGS} $^ -lm -o $@

tests/%.o: %.c tests
	${CC} ${TESTFLAGS} -c $< -o $@

tests/%.test.o: tests/%.test.c ext/munit/munit.h tests
	${CC} ${TESTFLAGS} -c $< -o $@

tests/%.test.c: %.h

lib/%.o: %.c
	${CC} ${BUILDFLAGS} -c $< -o $@

%.c: %.h

ext/munit/munit.o: ext/munit/munit.c ext/munit/munit.h
	${CC} -c $< -o $@

tests:
	mkdir -p tests

test: tests/test_filter tests/test_pll tests/test_buffer

clean:
	rm -rf tests/*.o
	rm -rf bin/*
	rm -rf lib/*
	rm -f *.so

.PHONY: clean tests test