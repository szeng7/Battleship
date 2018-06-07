CLAGS=-std=c11 -Wall -Wextra -Wpedantic -O2

all: battle

battle: battle.o commands.o stack.o 

battle.o: battle.c commands.h stack.h
commands.o: commands.c commands.h
stack.o: stack.c stack.h

test: all
	python piper.py battle battle > output.c
	sh testbattle.sh 2> errors.c
clean:
	rm -f battle *.o
