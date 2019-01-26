CC=clang
SRC=../jep_utils.c

# By default this install_name expects the library to be
# placed in the same directory as the executable using it.
#
# Change this to be the desired installation directory
# or remove it and put the library wherever
RPTH=-install_name ./libjep_utils.dylib

all:
	$(CC) -dynamiclib -Wall -Werror $(RPTH) $(SRC) -o ../libjep_utils.dylib

