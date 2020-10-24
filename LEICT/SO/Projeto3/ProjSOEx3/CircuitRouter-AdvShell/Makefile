# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2018-19

SOURCES = CircuitRouter-AdvShell.c 
SOURCES+= ../lib/commandlinereader.c ../lib/vector.c
OBJS = $(SOURCES:%.c=%.o)
CC   = gcc
CFLAGS =-Wall -std=gnu99 -I../
LDFLAGS=-lm
TARGET = CircuitRouter-AdvShell

all: $(TARGET) CircuitRouter-SeqSolver CircuitRouter-Client

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS) 

CircuitRouter-AdvShell.o: CircuitRouter-AdvShell.c CircuitRouter-AdvShell.h ../lib/commandlinereader.h ../lib/vector.h
../lib/commandlinereader.o: ../lib/commandlinereader.c ../lib/commandlinereader.h
../lib/vector.o: ../lib/vector.c ../lib/vector.h ../lib/types.h ../lib/utility.h

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $<

CircuitRouter-SeqSolver:
	(cd ../CircuitRouter-SeqSolver/ && make)

CircuitRouter-Client:
	(cd ../CircuitRouter-Client/ && make)

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGET)
	(cd ../CircuitRouter-SeqSolver/ && make clean)
	(cd ../CircuitRouter-Client/ && make clean) 
