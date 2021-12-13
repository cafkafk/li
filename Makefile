TARGET=li
SRC=li.c
PREFIX ?= /usr/local

CC=gcc
CFLAGS+=-O3 -march=native -std=c11 -Wall -W -pedantic -Wno-stringop-truncation
CPPFLAGS += -D_POSIX_C_SOURCE=1

.PHONY: all install uninstall debug clean 


all: $(TARGET)
debug: $(TARGET)-debug

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(SRC) -o $@

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

$(TARGET)-debug: $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(SRC) -Og -o $@

clean:
	$(RM) $(TARGET)
	$(RM) $(TARGET)-debug
