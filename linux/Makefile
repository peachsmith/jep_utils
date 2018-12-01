CC=gcc
SRC=../jep_utils.c
OBJ=jep_utils.o
OUT=../libjep_utils.so

all:
	$(CC) -c -Wall -Werror -fpic $(SRC)
	$(CC) -shared -o $(OUT) $(OBJ)
	rm jep_utils.o
