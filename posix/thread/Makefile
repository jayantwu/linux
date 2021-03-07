.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=01thread echosrv tsd pctest
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
01thread:01thread.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread


echosrv:echosrv.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread


tsd:tsd.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

pctest:pctest.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread


clean:
	rm -f *.o $(BIN)

