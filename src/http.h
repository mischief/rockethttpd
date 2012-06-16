#ifndef HTTP_H
#define HTTP_H

#include "defs.h"
#include "common_types.h"
#include "networking.h"
#include "http_helper.h"
#include "http_error.h"
#include "dir.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* this is a pointer to the data recieved from the client.
 * the length of the data recieved from the client.
 * this is a pointer to the memory that will store our data. */
int parse_http_request(char *, const size_t, connection *);

/*	this is a big guy.
	what this does is first urldecode the URL,
	and then reads the requested file into a buffer.
	at this point in time i do not know what will happen if
	the file cannot be read, either due to permissions or if it doesn't exist.. */

int fulfill_request(connection *);

/*	this guy is a helper to fulfill_request,
	it simply reads the file into a fat buffer. */

int get_that_file(connection *);

/*	this little guy is just here to clean up after all my malloc'ing.
	make sure this is called. it is responsible for EVERYTHING. */

void connection_destroy(connection *p);

/*	here is how we know what error happened!
	reads the status code, and then makes some text based on it.
	it kindly fills the data buffer, and the header for us. */

#endif
