OBJS = main.o pam_auth.o
HEADERS = pam_auth.h
CC = cc
CFLAGS = -Wall -pedantic
LDFLAGS = -lpam -lpam_misc

all: pam_c

pam_c: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJS) : %.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< -c
