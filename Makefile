OBJS = main.o authenticate.o counter.o word_reader.o
HEADERS = authenticate.h counter.h
CC = cc
CFLAGS = -Wall -pedantic -g -DAUTH_PAM_APP=korpusowe
LDFLAGS = -lpam -lpam_misc

.PHONY: all clean

all: pam_c

test: counter.o word_reader.o test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test.o: test.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< -c

pam_c: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJS) : %.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< -c

clean:
	rm -f $(OBJS) test.o test pam_c
