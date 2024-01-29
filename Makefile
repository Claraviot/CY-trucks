CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: process_t process_s

process_t: process_t.c
	$(CC) $(CFLAGS) process_t.c -o process_t

process_s: process_s.c
	$(CC) $(CFLAGS) process_s.c -o process_s

clean:
	rm -f process_t process_s

