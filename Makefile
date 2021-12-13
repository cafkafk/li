
CC=gcc
CFLAGS+=-O3 -march=native -std=c11 -Wall -Wno-stringop-truncation
lc: lc.c
	$(CC) -o lc $(CFLAGS) $<

debug-lc: lc.c
	$(CC) -o lc-debug -Og -g $<

check:
	echo "check"


clean:
	$(RM) lc

