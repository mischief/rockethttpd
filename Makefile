# -O0 for no optimization
CFLAGS = -Wall -O -D_POSIX_C_SOURCE=199506L -D_GNU_SOURCE

# final executeable name
OUTPUT = rockethttpd

VPATH = src

OBJECTS = main.o thread.o http.o networking.o

$(OUTPUT) : $(OBJECTS) -lpthread
	$(CC) $(CFLAGS) -o $@ $^
     
main.o : main.c thread.h common_types.h networking.h defs.h
	$(CC) -c $(CFLAGS) $< -o $@
thread.o : thread.c thread.h http.h networking.h common_types.h defs.h
	$(CC) -c $(CFLAGS) $< -o $@
http.o : http.c http.h common_types.h networking.h defs.h
	$(CC) -c $(CFLAGS) $< -o $@
networking.o : networking.c networking.h common_types.h
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean :
	$(RM) $(OUTPUT) *.o
