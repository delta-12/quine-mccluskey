.PHONY = all clean

CPPC = g++
CPPFLAGS = -Wall -D_THREAD_SAFE
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
BINS = $(patsubst %.cpp, %, $(SRCS))

all: FileHandler.o Minterms.o quine-mccluskey

FileHander.o: FileHandler.cpp FileHandler.h
	${CPPC} ${CPPLAGS} -o FileHandler.o -c FileHandler.cpp

Minterms.o: Minterms.cpp Minterms.h
	${CPPC} ${CPPLAGS} -o Minterms.o -c Minterms.cpp

quine-mccluskey: quine-mccluskey.cpp FileHandler.o Minterms.o
	${CPPC} ${CPPFLAGS} -o $@ $^

clean:
	@echo "Cleaning up..."
	@rm -rvf ${BINS} ${OBJS}