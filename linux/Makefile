CC=gcc
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

OUT=libjep_utils.so
TEST_OUT=tests

all:
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/bitstring.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/byte_buffer.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/char_buffer.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/character.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/string.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/unicode.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/huffman.c
	$(CC) -c -Wall -fpic -I$(INC) $(SRC)/json.c

	$(CC) -shared -o $(OUT) $(OBJ)
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
