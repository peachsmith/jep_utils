CC=clang
INC=../include
SRC=../src
TEST_SRC=../tests
TEST_INC=../tests

OBJ=bitstring.o \
byte_buffer.o  \
char_buffer.o  \
character.o    \
string.o       \
unicode.o      \
huffman.o      \
json.o

TEST_OBJ=bitstring_tests.o \
byte_buffer_tests.o  \
char_buffer_tests.o  \
character_tests.o    \
string_tests.o       \
unicode_tests.o      \
huffman_tests.o      \
json_tests.o         \
main.o

OUT=libjep_utils.dylib
TEST_OUT=tests

# By default this install_name expects the library to be
# placed in the same directory as the executable using it.
#
# Change this to be the desired installation directory
# or remove it and put the library wherever
INSTALL_DIR=~/Documents/CLibs/jep_utils

RPTH=-install_name $(INSTALL_DIR)/libjep_utils.dylib

# all:
# 	$(CC) -dynamiclib -Wall -Werror $(RPTH) $(SRC) -o $(OUT)
# 	$(CC) -Wall -Werror -I.. $(SRC) $(TEST_SRC) -o $(TEST_OUT)

all:
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/bitstring.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/byte_buffer.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/char_buffer.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/character.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/string.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/unicode.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/huffman.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/json.c

	$(CC) -dynamiclib -o $(OUT) $(OBJ)
	rm *.o

test:
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/bitstring_tests.c
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/byte_buffer_tests.c
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/char_buffer_tests.c
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/character_tests.c
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/string_tests.c
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/unicode_tests.c
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/huffman_tests.c
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/json_tests.c
	$(CC) -c -Wall -fpic -I$(INC) -I$(TEST_INC) $(TEST_SRC)/main.c

	$(CC) -o $(TEST_OUT) $(TEST_OBJ) -L. -ljep_utils -Wl,-rpath,.
	rm *.o

install:
	cp $(OUT) $(INSTALL_DIR)
