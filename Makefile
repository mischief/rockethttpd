# -O0 for no optimization
CFLAGS = -Wall -O -D_POSIX_C_SOURCE=199506L -D_GNU_SOURCE

# final executeable name
OUTPUT = rockethttpd

VPATH = src

OBJECTS = main.o thread.o dir.o http.o http_helper.o http_error.o networking.o data_templates.o

$(OUTPUT) : $(OBJECTS) -lpthread
	$(CC) $(CFLAGS) $^ -o $@

main.o : main.c thread.h common_types.h networking.h defs.h
	$(CC) -c $(CFLAGS) $< -o $@
thread.o : thread.c thread.h http.h networking.h common_types.h defs.h
	$(CC) -c $(CFLAGS) $< -o $@
http.o : http.c http.h common_types.h networking.h defs.h
	$(CC) -c $(CFLAGS) $< -o $@
http_helper.o : http_helper.c defs.h common_types.h
	$(CC) -c $(CFLAGS) $< -o $@
http_error.o : http_error.c defs.h common_types.h http_helper.h
	$(CC) -c $(CFLAGS) $< -o $@
networking.o : networking.c networking.h common_types.h
	$(CC) -c $(CFLAGS) $< -o $@
dir.o : dir.c dir.h defs.h
	$(CC) -c $(CFLAGS) $< -o $@
data_templates.o : data_templates.c common_types.h
	$(CC) -c $(CFLAGS) $< -o $@

debug : $(OBJECTS) -lpthread
	$(CC) $(CFLAGS) -g $^ -o debug

.PHONY: clean
clean :
	$(RM) $(OUTPUT) *.o
