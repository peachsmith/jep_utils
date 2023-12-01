#=============================================================================
# Makefile for Linux
#=============================================================================

CC=gcc
CFLAGS=-Wall -Werror
LIBS=-ljep_utils

INCLUDE_DIR=-I../include/jep_utils

SRC=../src/*.c
TEST_SRC=../tests/*.c

all:
	$(CC) -fPIC -shared  $(SRC) -DJEP_UTILS_EXPORTS -o libjep_utils.so $(INCLUDE_DIR)

debug:
	$(CC) -g -O0 -fPIC -shared $(SRC) -DJEP_UTILS_EXPORTS -o libjep_utils.so $(INCLUDE_DIR)

test:
	$(CC) $(TEST_SRC) -o tests.out $(INCLUDE_DIR) -I../tests -L. $(LIBS)

test-debug:
	$(CC) -g -O0 $(TEST_SRC) -o tests.out $(INCLUDE_DIR) -I../tests -L. $(LIBS)
