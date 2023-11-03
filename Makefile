CC=gcc
CFLAGS=-g -Wall -Wpedantic -Wextra -Werror -std=c2x -I${INCLUDE} 

LIB_SOURCE_DIR=src/lib
LIB_SOURCE=${wildcard ${LIB_SOURCE_DIR}/*.c}
LIB_OBJECTS=$(patsubst %.c,%.o,${LIB_SOURCE})
INCLUDE=include

TEST_SOURCE_DIR=src/test

lib/libquickwave.a: ${LIB_OBJECTS}
	ar rcs $@ $^

tests/test_%: ${TEST_SOURCE_DIR}/%.test.o lib/libquickwave.a ext/munit/munit.o
	$(CC) ${CFLAGS} $^ -lm -o $@

${TEST_SOURCE_DIR}/%.o: ${TEST_SOURCE_DIR}/%.c ${TEST_SOURCE_DIR}/test.h
	$(CC) ${CFLAGS} -c -Iext/munit $< -o $@

${LIB_SOURCE_DIR}/%.o: ${LIB_SOURCE_DIR}/%.c ${LIB_SOURCE_DIR}/%.h
	$(CC) ${CFLAGS} -c $< -o $@

ext/munit/munit.o: ext/munit/munit.c ext/munit/munit.h
	$(CC) -c $< -o $@

tests/iq.csv tests/const_freq.csv tests/sweep.csv &: tests/test_pll
	./tests/test_pll

tests/iir_response.csv tests/test_sinc.csv &: tests/test_filter
	./tests/test_filter

tests/%.pdf: ${TEST_SOURCE_DIR}/plot.plt ${TEST_SOURCE_DIR}/%.plt tests/%.csv 
	gnuplot -c  $^ $@

.PHONY: clean test plots

test: tests/test_filter tests/test_pll tests/test_buffer

plots: tests/iq.pdf tests/const_freq.pdf tests/sweep.pdf

clean:
	rm -rf tests/*
	rm -rf bin/*
	rm -rf lib/*
	rm -rf src/lib/*.o
	rm -rf src/test/*.o
