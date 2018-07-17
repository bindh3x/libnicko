CC = cc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -shared
LDLIBS = -ltcl -lnicko

all: tcl-nicko

tcl-nicko: tcl-nicko.o
	$(CC) $(CFLAGS) -o nicko.so tcl-nicko.o $(LDLIBS) $(LDFLAGS)

clean:
	rm -f nicko.so *.o
