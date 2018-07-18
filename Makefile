AR     ?= ar
RANLIB ?= ranlib
CFLAGS = -Wall -Wextra
OBJECTS = $(SRCS:.c=.o)
PREFIX  ?= /usr/local

SRCS    =  nicko.c

all: lib nicko

lib: libnicko.a

libnicko.a: $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)
	$(RANLIB) $@

nicko: lib cli.o
	$(CC) $(CFLAGS) -o $@ cli.o libnicko.a

install: all
	install -d $(PREFIX)/lib
	install -m 0644 libnicko.a $(PREFIX)/lib
	install -d $(PREFIX)/include
	install -m 0644 nicko.h $(PREFIX)/include/nicko.h
	install -d $(PREFIX)/bin
	install -m755 nicko $(PREFIX)/bin/nicko
	install -d $(PREFIX)/share/man1
	gzip -c < nicko.1 > $(PREFIX)/share/man1/nicko.1.gz

uninstall:
	rm -f $(PREFIX)/lib/libnicko.a
	rm -f $(PREFIX)/include/nicko.h
	rm -f $(PREFIX)/bin/nicko $(PREFIX)/share/man1/nicko.1.gz

clean:
	rm -f *.o libnicko.a nicko
