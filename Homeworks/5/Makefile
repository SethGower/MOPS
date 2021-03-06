# dynamic directories
INCLUDE_DIR =include
OBJECT_DIR=build
OUTPUT_DIR=out

# parameters
CC=gcc
CFLAGS=-I$(INCLUDE_DIR) -std=c99 -pedantic -Wall -Wextra -g
LIBS=-lm

# output
OUTPUT = $(OUTPUT_DIR)/bst

TEST_FILE = ./5321467


# object files
_OBJECTS = bst.o
OBJECTS = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJECTS))

_HEADERS = bst.h
HEADERS = $(patsubst %,$(INCLUDE_DIR)/%,$(_HEADERS))

#define compile rule for object files. If a C file changes, generate its .obj under /obj.
$(OBJECT_DIR)/%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

# all: Generate output, using compiled .obj files
all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) -o $(OUTPUT) $^ $(CFLAGS) $(LIBS)


# clean: remove all object files and output, use -f option.
.PHONY: clean run help gdb
clean:
	rm -f $(OBJECT_DIR)/*.o $(OUTPUT)

# run valgrind to check for any issues
mem: all
	valgrind --tool=memcheck --leak-check=full ./$(OUTPUT) < $(TEST_FILE)> /dev/null

run: all
	$(OUTPUT) < $(TEST_FILE)

gdb: all
	gdb $(OUTPUT)

# help: list available make options
help:
	@echo "Make options:  all, mem, clean, help"
