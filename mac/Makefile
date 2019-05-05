CC=clang
SRC=../jep_utils.c
OUT=../libjep_utils.dylib

# By default this install_name expects the library to be
# placed in the same directory as the executable using it.
#
# Change this to be the desired installation directory
# or remove it and put the library wherever
INSTALL_DIR=~/Documents/CLibs/jep_utils

RPTH=-install_name $(INSTALL_DIR)/libjep_utils.dylib

all:
	$(CC) -dynamiclib -Wall -Werror $(RPTH) $(SRC) -o $(OUT)

install:
	cp $(OUT) $(INSTALL_DIR)
