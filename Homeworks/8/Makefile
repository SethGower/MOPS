# dynamic directories
INCLUDE_DIR =./include
OBJECT_DIR=./build
OUTPUT_DIR=./out

# parameters
CC=gcc
CFLAGS=-I$(INCLUDE_DIR) -std=c99 -pedantic -Wall -Wextra -g -pthread
LIBS=-lm

VAL = valgrind --tool=memcheck --leak-check=full -s

# output
OUTPUT = $(OUTPUT_DIR)/pt-cruisers

MEM_OUT = $(OUTPUT_DIR)/mem.txt

# object files
_OBJECTS = racer.o pt-cruisers.o display.o
OBJECTS = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJECTS))

#define compile rule for object files. If a C file changes, generate its .obj under /obj.
$(OBJECT_DIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# all: Generate output, using compiled .obj files
all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $(OUTPUT)

# clean: remove all object files and output, use -f option.
.PHONY: clean run help gdb
clean:
	- rm -f $(OBJECT_DIR)/*.o $(OUTPUTS) /out.* $(MEM_OUT)

# run valgrind to check for any issues
mem: all
	@echo "Testing '$(OUTPUT) hello there' for memory violations"> $(MEM_OUT)
	@echo "===================================================================" >>$(MEM_OUT)
	- $(VAL) $(OUTPUT) hello there> /dev/null 2>> $(MEM_OUT)
	@cat $(MEM_OUT)
	@echo "Testing '$(OUTPUT) ./file-bitsets.c there' for memory violations">> $(MEM_OUT)
	@echo "===================================================================" >>$(MEM_OUT)
	- $(VAL) $(OUTPUT) file-bitsets.c there> /dev/null 2>> $(MEM_OUT)
	@cat $(MEM_OUT)

run: all
	$(OUTPUT) 100 fuck me up


gdb: all
	gdb $(OUTPUT)

# help: list available make options
help:
	@echo "Make options:  all, mem, clean, help"
