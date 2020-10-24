# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2018-19

SOURCES = CircuitRouter-Client.c
OBJS = $(SOURCES:%.c=%.o)
CC   = gcc
CFLAGS =-Wall -std=gnu99 -I../
LDFLAGS=-lm
TARGET = CircuitRouter-Client

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS) 

CircuitRouter-Client.o: CircuitRouter-Client.c

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGET)
