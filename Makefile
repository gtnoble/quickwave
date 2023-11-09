CC=gcc
CFLAGS=-g -Wall -Wpedantic -Wextra -Werror -std=c99 -I${INCLUDE_DIR} 

LIB_SOURCE_DIR=src/lib
LIB_SOURCE=${wildcard ${LIB_SOURCE_DIR}/*.c}
LIB_OBJECTS=$(patsubst %.c,%.o,${LIB_SOURCE})
INCLUDE_DIR=include

TEST_SOURCE_DIR=src/test

lib/libquickwave.a: ${LIB_OBJECTS}
	ar rcs $@ $^

tests/test_%: ${TEST_SOURCE_DIR}/%.test.o lib/libquickwave.a ext/munit/munit.o
	$(CC) ${CFLAGS} $^ -lm -o $@

${TEST_SOURCE_DIR}/%.o: ${TEST_SOURCE_DIR}/%.c ${TEST_SOURCE_DIR}/test.h
	$(CC) ${CFLAGS} -c -Iext/munit $< -o $@

${LIB_SOURCE_DIR}/%.o: ${LIB_SOURCE_DIR}/%.c ${INCLUDE_DIR}/%.h
	$(CC) ${CFLAGS} -c $< -o $@

ext/munit/munit.o: ext/munit/munit.c ext/munit/munit.h
	$(CC) -c $< -o $@

tests/iq.csv tests/const_freq.csv tests/sweep.csv &: tests/test_pll
	./tests/test_pll

tests/iir_response.csv tests/test_sinc.csv &: tests/test_filter
	./tests/test_filter

tests/linear_fit.csv &: tests/test_linear_model
	./tests/test_linear_model

tests/sinusoid_fit.csv &: tests/test_sinusoid_fit
	./tests/test_sinusoid_fit

tests/%.pdf: ${TEST_SOURCE_DIR}/plot.plt ${TEST_SOURCE_DIR}/%.plt tests/%.csv 
	gnuplot -c  $^ $@

.PHONY: clean test plots

test: tests/test_filter tests/test_pll tests/test_buffer tests/test_linear_model

plots: tests/iq.pdf tests/const_freq.pdf tests/sweep.pdf tests/iir_response.pdf tests/test_sinc.pdf tests/sinusoid_fit.pdf

install:
	cp lib/* /usr/local/lib
	mkdir -p /usr/local/include/quickwave
	cp include/* /usr/local/include/quickwave

clean:
	rm -rf tests/*
	rm -rf bin/*
	rm -rf lib/*
	rm -rf src/lib/*.o
	rm -rf src/test/*.o
