TARGET=li
SRC=li.c
PREFIX ?= /usr/local

CC=gcc
CFLAGS+=-O3 -march=native -std=c11 -Wall -W -pedantic -Wno-stringop-truncation
CFLAGSREL+=-O3 -std=c11 -Wall -W -pedantic -Wno-stringop-truncation
CPPFLAGS += -D_POSIX_C_SOURCE=1

.PHONY: all release install uninstall debug clean


all: $(TARGET)
release: $(TARGET)-release
debug: $(TARGET)-debug

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(SRC) -o $@

$(TARGET)-release: $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGSREL) $(LDFLAGS) $(SRC) -o li

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
