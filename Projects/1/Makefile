# dynamic directories
INCLUDE_DIR =include
OBJECT_DIR=build
OUTPUT_DIR=out

# parameters
CC=gcc
CFLAGS=-I$(INCLUDE_DIR) -std=c99 -pedantic -Wall -Wextra -g
LIBS=-lm

# output
OUTPUT = $(OUTPUT_DIR)/Bracetopia


# object files
_OBJECTS = bracetopia.o driver.o
OBJECTS = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJECTS))

#define compile rule for object files. If a C file changes, generate its .obj under /obj.
$(OBJECT_DIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# all: Generate output, using compiled .obj files
all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) -o $(OUTPUT) $^ $(CFLAGS) $(LIBS)


# clean: remove all object files and output, use -f option.
.PHONY: clean run help
clean:
	rm -f $(OBJECT_DIR)/*.o $(OUTPUT)

# run valgrind to check for any issues
mem: all
	valgrind ./$(OUTPUT)

run: all
	$(OUTPUT)

# help: list available make options
help:
	@echo "Make options:  all, mem, clean, help"
