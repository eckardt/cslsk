#        $Id: Makefile,v 1.13 2005/02/02 19:06:42 se Exp $

CC = gcc
CFLAGS = -g $(DRY) -Wall -Werror -I/usr/pkg/include
LDFLAGS = -L/usr/lib -ledit -ltermcap -lz
OBJ = cslsk.o netio.o msg.o mksmsg.o prsmsg.o prpmsg.o interface.o

VERSION = 0.00

all: cslsk

tgz:
	(cd ../ && tar -zcvf cslsk-$(VERSION).tar.gz cslsk/)

dry:
	make all DRY=-DDRY

cslsk.o: cslsk.c
	$(CC) $(CFLAGS) -c -o cslsk.o cslsk.c

netio.o: netio.c
	$(CC) $(CFLAGS) -c -o netio.o netio.c

msg.o: msg.c
	$(CC) $(CFLAGS) -c -o msg.o msg.c

mksmsg.o: mksmsg.c
	$(CC) $(CFLAGS) -c -o mksmsg.o mksmsg.c

prsmsg.o: prsmsg.c
	$(CC) $(CFLAGS) -c -o prsmsg.o prsmsg.c

prpmsg.o: prpmsg.c
	$(CC) $(CFLAGS) -c -o prpmsg.o prpmsg.c

interface.o: interface.c
	$(CC) $(CFLAGS) -c -o interface.o interface.c

cslsk: $(OBJ)
	$(CC) -o cslsk $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OBJ) cslsk cslsk.core
