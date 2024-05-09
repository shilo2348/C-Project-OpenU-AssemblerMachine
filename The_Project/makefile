COMPILE := gcc -g -Wall -pedantic -ansi
OFILES := $(patsubst %.c, %.o, $(wildcard ./*.c))

all: assembler

assembler: $(OFILES)
	$(COMPILE) -o assembler $(OFILES)

%.o: %.c %.h
	$(COMPILE) -c $< -o $@

clean:
	rm -f assembler $(OFILES)

val:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./assembler input1
