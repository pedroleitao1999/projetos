TARGETS = file-sender file-receiver

CC = gcc
#CFLAGS = -Wall -Werror -O3

default: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
