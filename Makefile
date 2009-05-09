CFLAGS = -Wall -O -DDEBUG=0 -D_POSIX_C_SOURCE=199506L -D_GNU_SOURCE

# final executeable name
OUTPUT = rockethttpd

VPATH = src

OBJECTS = main.o thread.o dir.o http.o http_helper.o http_error.o networking.o

LIBS = -lpthread -lm

$(OUTPUT) : $(OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

debug : CFLAGS = -Wall -g -DDEBUG=1 -D_POSIX_C_SOURCE=199506L -D_GNU_SOURCE
debug : $(OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

gprof : CFLAGS = -Wall -pg -DDEBUG=0 -D_POSIX_C_SOURCE=199506L -D_GNU_SOURCE
gprof : $(OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.c thread.h networking.h
	$(CC) -c $(CFLAGS) -o $@ $<
thread.o : thread.c thread.h http.h networking.h
	$(CC) -c $(CFLAGS) -o $@ $<
http.o : http.c http.h networking.h
	$(CC) -c $(CFLAGS) -o $@ $<
http_helper.o : http_helper.c
	$(CC) -c $(CFLAGS) -o $@ $<
http_error.o : http_error.c http_helper.h
	$(CC) -c $(CFLAGS) -o $@ $<
networking.o : networking.c networking.h
	$(CC) -c $(CFLAGS) -o $@ $<
dir.o : dir.c dir.h
	$(CC) -c $(CFLAGS) -o $@ $<
$(OBJECTS) : defs.h common_types.h


.PHONY: clean
clean :
	$(RM) $(OUTPUT) debug gprof *.o
