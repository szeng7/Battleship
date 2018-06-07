CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -O2 -g --coverage
LDFLAGS=--coverage

all: battle test_driver

test_driver: test_driver.o commands.o
test_driver.o: test_driver.c commands.h

battle: battle.o commands.o
battle.o: battle.c commands.h

commands.o: commands.c commands.h

test: all
	python piper.py battle battle > output.c
	sh testbattle.sh 2> errors.c
	./test_driver 2> errors2.c
	gcov test_driver.c
	gcov battle.c
	gcov commands.c
clean:
	rm -f battle test_driver *.o *.gcno *.gcda *.gcov

