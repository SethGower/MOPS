# Makefile for activity 1
# 

CFLAGS_1 = -Wno-format -g -ggdb -std=c99

all:
	@echo command choices:
	@echo make partA
	@echo or...
	@echo make partB
	@echo or...
	@echo make clean

# part A debugging
partA:
	@-rm -f Part*
	@gcc $(CFLAGS_1) good-life.c -o good-life
	@-touch PartA


CFLAGS_2 = -g -ggdb -std=c99

# part B debugging
partB:
	@-rm -f Part*
	gcc $(CFLAGS_2) good-life.c -o good-life
	@-touch PartB

clean:
	-rm -f good-life
	@-rm -f Part*

