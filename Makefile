CFLAGS=-g -Wall -Wpedantic -Wextra -Werror -std=c11 -I${INCLUDE_DIR} -I${FFT_INCLUDE_DIR}

LIB_SOURCE_DIR=src/lib
LIB_SOURCE=${wildcard ${LIB_SOURCE_DIR}/*.c}
LIB_OBJECTS=$(patsubst %.c,%.o,${LIB_SOURCE})
INCLUDE_DIR=include
INCLUDE_SOURCE_DIR=src/include

FFT_BIN_DIR=ext/fft
FFT_SOURCE_DIR=ext/fft/src
FFT_INCLUDE_DIR=ext/fft/include

TEST_SOURCE_DIR=src/test

GUILE_SOURCE_DIR=scheme
export GUILE_LOAD_PATH=${GUILE_SOURCE_DIR}
CODE_GENERATION_DEPENDENCIES=${GUILE_SOURCE_DIR}/substitutions.scm ${GUILE_SOURCE_DIR}/template.scm
HEADER_SOURCE_DIR=src/include
HEADER_SOURCE=$(wildcard ${HEADER_SOURCE_DIR}/*.h.scm)
GENERATED_HEADERS=$(patsubst ${HEADER_SOURCE_DIR}/%.h.scm,${INCLUDE_DIR}/%.h,${HEADER_SOURCE})

lib/libquickwave.a: ${LIB_OBJECTS} ${FFT_BIN_DIR}/ooura_fft.o
	ar rcs $@ $^

tests/test_%: ${TEST_SOURCE_DIR}/%.test.o lib/libquickwave.a ext/munit/munit.o 
	$(CC) $(CFLAGS) $^ -lm -o $@

${TEST_SOURCE_DIR}/%.o: ${TEST_SOURCE_DIR}/%.c ${TEST_SOURCE_DIR}/test.h
	$(CC) $(CFLAGS) -c -Iext/munit $< -o $@

${LIB_SOURCE_DIR}/%.o: ${LIB_SOURCE_DIR}/%.c ${INCLUDE_DIR}/%.h ${GENERATED_HEADERS}
	$(CC) $(CFLAGS) -c $< -o $@

ext/munit/munit.o: ext/munit/munit.c ext/munit/munit.h
	$(CC) $(CFLAGS) -c $< -o $@

#Code generation

${FFT_INCLUDE_DIR}/%.h: ${FFT_INCLUDE_DIR}/%.h.scm ${CODE_GENERATION_DEPENDENCIES}
	guile -s $< > $@

${FFT_SOURCE_DIR}/%.c: ${FFT_SOURCE_DIR}/%.c.scm ${CODE_GENERATION_DEPENDENCIES}
	guile -s $< > $@

${FFT_BIN_DIR}/ooura_fft.o: ${FFT_SOURCE_DIR}/fft4g.c ${FFT_INCLUDE_DIR}/fftg.h
	$(CC) $(CFLAGS) -c $< -o $@

${LIB_SOURCE_DIR}/%.c: ${LIB_SOURCE_DIR}/%.c.scm ${CODE_GENERATION_DEPENDENCIES}
	guile -s $< > $@

${INCLUDE_DIR}/%.h: ${INCLUDE_SOURCE_DIR}/%.h.scm ${CODE_GENERATION_DEPENDENCIES}
	guile -s $< > $@

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

.PHONY: test
test: tests/test_filter tests/test_pll tests/test_buffer tests/test_linear_model tests/test_fft

.PHONY: plots
plots: tests/iq.pdf tests/const_freq.pdf tests/sweep.pdf tests/iir_response.pdf tests/test_sinc.pdf tests/sinusoid_fit.pdf

.PHONY: headers
headers: ${GENERATED_HEADERS}

.PHONY: install
install: lib/libquickwave.a headers
	cp lib/* /usr/local/lib
	mkdir -p /usr/local/include/quickwave
	cp include/* /usr/local/include/quickwave

.PHONY: clean
clean:
	rm -rf tests/*
	rm -rf bin/*
	rm -rf lib/*
	rm -rf src/lib/*.o
	rm -rf src/test/*.o
