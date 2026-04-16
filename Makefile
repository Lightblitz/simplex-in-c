# name of executable
EXEC = program

# compiler to use
CC = clang

# command-line options
CFLAGS = -std=c99 -g -Wall -MMD

# Libraries to link (NEW)
LDLIBS = -lncurses

SRC = $(wildcard *.c)
OBJECTS = $(SRC:.c=.o)
DEPENDS = $(OBJECTS:.o=.d)

${EXEC}: ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${EXEC} ${LDLIBS}

# copy the generated .d files
-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f *.o *.d ${EXEC}