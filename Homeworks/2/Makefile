CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -Werror -pedantic -I $(INC_DIR) -lm
INC_DIR = ./include
OUT_DIR = ./out
BUILD_DIR = ./build
VAL=valgrind --tool=memcheck --leak-check=yes --track-origins=yes

HEADERS = $(wildcard $(INC_DIR)/*.h)
SRC = ./hist.c
PROG = $(OUT_DIR)/hist
RESULT = $(OUT_DIR)/output.txt

ifeq ($(strip $(BUILD_DIR)), )
	OBJ_FOLDER =
else
	OBJ_FOLDER = $(strip $(BUILD_DIR))/
endif
ifeq ($(strip $(BUILD_DIR)), )
	OBJ_FOLDER =
else
	OBJ_FOLDER = $(strip $(BUILD_DIR))/
endif
ifeq ($(strip $(OUT_DIR)), )
	BIN_FOLDER =
else
	BIN_FOLDER = $(strip $(OUT_DIR))/
endif

all: create_outputdir $(PROG)

.PHONY: clean tests help mem gdb
clean:
	- rm -rf $(OUT_DIR)/*
	- rm -rf $(OBJ_FOLDER)*

help:
	@echo "The available targets are: [all],clean,test,mem,gdb"

gdb: all
	gdb $(PROG)

meme: all
	- $(VAL) $(PROG) -b -t 1e-6 -g -2.5 -u 2.5 &> $(OUT_DIR)/mem.txt
	- $(VAL) $(PROG) -n -t 1e-6 -g -1.5  &>> $(OUT_DIR)/mem.txt
	- $(VAL) $(PROG) -s -t 1e-6 -g -1.5  &>> $(OUT_DIR)/mem.txt
	- $(VAL) $(INT_PROG) &>> $(OUT_DIR)/mem.txt
	- $(VAL) $(QUAD_PROG) &>> $(OUT_DIR)/mem.txt
	@cat $(OUT_DIR)/mem.txt
	@cat $(BIN_FOLDER)mem.txt

mem: meme

test: all
	@echo Testing with hist.c
	- $(PROG) < hist.c
	@echo =========================================
	@echo Testing with box
	- $(PROG) < ./texts/box
	@echo =========================================
	@echo Testing with fox
	- $(PROG) < ./texts/fox
	@echo =========================================
	@echo Testing with empty
	- $(PROG) < ./texts/empty

create_outputdir:
	$(shell mkdir $(OBJ_FOLDER) 2> /dev/null)
	$(shell mkdir $(BIN_FOLDER) 2> /dev/null)

$(PROG): $(SRC) $(HEADERS)
	@echo 'Building target: $@'
	$(CC) -o $@ $(SRC) $(CFLAGS)
	@echo 'Finished Building Target: $@'
	@echo ' '
