OBJS = main.o authenticate.o counter.o word_reader.o
HEADERS = authenticate.h counter.h
CC = cc
CFLAGS = -Wall -pedantic -std=c99
LDFLAGS = -lpam -lpam_misc

.PHONY: all clean

all: korpusowe

test: counter.o word_reader.o test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test.o: test.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< -c

korpusowe: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJS) : %.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< -c

clean:
	rm -f $(OBJS) test.o test korpusowe
