CC=gcc
CFLAGS=-g -Wall -Wpedantic
TESTFLAGS=${CFLAGS} -DSQLITE_CORE 
BUILDFLAGS=${CFLAGS} -fPIC

libsqldsp.so: lib/main.o lib/filter.o lib/functions.o lib/buffer.o lib/savgol.o
	${CC} ${BUILDFLAGS} -shared lib/functions.o lib/buffer.o lib/filter.o lib/savgol.o lib/main.o -o libsqldsp.so

tests/test_%: tests/%.o tests/%.test.o tests
	${CC} ${TESTFLAGS} -c $^ -lsqlite3 -o $@

tests/test_filter: tests/filter.test.o tests/filter.o tests/savgol.o tests
	${CC} ${TESTFLAGS} tests/filter.test.o tests/filter.o tests/buffer.o tests/savgol.c -lsqlite3 -o tests/test_filter

tests/%.o: %.c
	${CC} ${TESTFLAGS} -c $< -o $@

tests/%.test.o: tests/%.test.c
	${CC} ${TESTFLAGS} -c $< -o $@

tests/%.test.c: %.h

lib/%.o: %.c
	${CC} ${BUILDFLAGS} -c $< -o $@

%.c: %.h

tests:
	mkdir -p tests

test: tests tests/test_filter tests/test_%

clean:
	rm -rf tests/*.o
	rm -rf bin/*
	rm -rf lib/*
	rm -f *.so

.PHONY: clean tests test