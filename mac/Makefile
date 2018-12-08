CC=clang
SRC=../jep_utils.c

# change this to be the desired installation directory
# or remove it and put the library wherever
RPTH=-install_name ./libjep_utils.dylib

all:
	$(CC) -dynamiclib -Wall -Werror $(RPTH) $(SRC) -o ../libjep_utils.dylib

