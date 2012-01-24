XTRAFLAGS =
OPTIMIZATION =-O3 -Os
GPROFFLAG =-pg -g -DDEBUG=1 -fprofile-arcs -ftest-coverage
DEBUGFLAG =-g -DDEBUG=1
# -pthread sets macros and sets linker flags
PTHREAD =-pthread
# -march parameter
MARCH =-march=native
# link with math library
LIBS =-lm
DEFINES =-D_POSIX_C_SOURCE=199506L -D_BSD_SOURCE
#DEFINES =-D_POSIX_C_SOURCE=199506L -D_GNU_SOURCE
CFLAGS :=-Wall -fstack-protector -std=gnu99 $(DEFINES) $(MARCH) $(PTHREAD) $(XTRAFLAGS)
# final executeable name
OUTPUT=rockethttpd

VPATH = src

OBJECTS = rockethttpd.o thread.o dir.o http.o http_helper.o http_error.o networking.o config.o

$(OUTPUT) : CFLAGS := $(CFLAGS) $(OPTIMIZATION)
$(OUTPUT) : $(OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

# debug build with -g -DDEBUG=1
debug : CFLAGS := $(CFLAGS) $(DEBUGFLAG)
debug : $(OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

# build for use with gprof, using -pg flag
gprof : CFLAGS := $(CFLAGS) $(GPROFFLAG)
gprof : $(OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

# fake
fake : CFLAGS := $(CFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean
clean :
	$(RM) $(OUTPUT) debug gprof *.o *.gcno
