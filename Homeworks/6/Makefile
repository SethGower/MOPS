# dynamic directories
INCLUDE_DIR =./include
OBJECT_DIR=./build
OUTPUT_DIR=./out

# parameters
CC=gcc
CFLAGS=-I$(INCLUDE_DIR) -std=c99 -pedantic -Wall -Wextra -g
LIBS=-lm

VAL = valgrind --tool=memcheck --leak-check=full -s

# output
OUTPUT1 = $(OUTPUT_DIR)/mapTest1
OUTPUT2 = $(OUTPUT_DIR)/mapTest2
OUTPUT3 = $(OUTPUT_DIR)/mapTest3
OUTPUT4 = $(OUTPUT_DIR)/mytest
OUTPUTS =  $(OUTPUT1) $(OUTPUT2) $(OUTPUT3)
TEST_SCRIPT = ./RUN

MEM_OUT = $(OUTPUT_DIR)/mem.txt

# object files
_OBJECTS = mapTest1.o mapTest2.o mapTest3.o mapADT.o mytest.o
OBJECTS = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJECTS))

_HEADERS = mapADT.h
HEADERS = $(patsubst %,$(INCLUDE_DIR)/%,$(_HEADERS))

#define compile rule for object files. If a C file changes, generate its .obj under /obj.
$(OBJECT_DIR)/%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

# all: Generate output, using compiled .obj files
all: $(OUTPUTS)

$(OUTPUT1): $(word 4, $(OBJECTS)) $(word 1, $(OBJECTS))
	$(CC) $(CFLAGS) $(LIBS) $(word 4, $(OBJECTS)) $(word 1, $(OBJECTS)) -o $(word 1, $(OUTPUTS))

$(OUTPUT2): $(word 4, $(OBJECTS)) $(word 2, $(OBJECTS))
	$(CC) $(CFLAGS) $(LIBS) $(word 4, $(OBJECTS)) $(word 2, $(OBJECTS)) -o $(word 2, $(OUTPUTS))

$(OUTPUT3): $(word 4, $(OBJECTS)) $(word 3, $(OBJECTS))
	$(CC) $(CFLAGS) $(LIBS) $(word 4, $(OBJECTS)) $(word 3, $(OBJECTS)) -o $(word 3, $(OUTPUTS))

$(OUTPUT4): $(word 4, $(OBJECTS)) $(word 5, $(OBJECTS))
	$(CC) $(CFLAGS) $(LIBS) $(word 4, $(OBJECTS)) $(word 5, $(OBJECTS)) -o $(word 4, $(OUTPUTS))

# clean: remove all object files and output, use -f option.
.PHONY: clean run help gdb
clean:
	- rm -f $(OBJECT_DIR)/*.o $(OUTPUTS) /out.* $(MEM_OUT)

# run valgrind to check for any issues
mem: all
	@echo "Testing mapTest1 for memory violations" > $(MEM_OUT)
	@echo "===================================================================" >>$(MEM_OUT)
	$(VAL) ./$(OUTPUT1) > /dev/null 2> $(MEM_OUT)
	@echo "Testing mapTest2 for memory violations" >> $(MEM_OUT)
	@echo "===================================================================" >>$(MEM_OUT)
	$(VAL) ./$(OUTPUT2) > /dev/null 2>> $(MEM_OUT)
	@echo "Testing mapTest3 for memory violations" >> $(MEM_OUT)
	@echo "===================================================================" >>$(MEM_OUT)
	$(VAL) ./$(OUTPUT3) > /dev/null 2>> $(MEM_OUT)
	# @echo "Testing mytest for memory violations" >> $(MEM_OUT)
	# @echo "===================================================================" >>$(MEM_OUT)
	# $(VAL) ./$(OUTPUT4) > /dev/null 2>> $(MEM_OUT)
	@cat $(MEM_OUT)

run: all
	cd $(OUTPUT_DIR);$(TEST_SCRIPT)


gdb: all
	gdb $(word 3, $(OUTPUTS))

# help: list available make options
help:
	@echo "Make options:  all, mem, clean, help"
