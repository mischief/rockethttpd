#ifndef HTTP_H
#define HTTP_H

#include <string.h>
#include "common_types.h"

const int parse_http_request(	char *,		// this is a pointer to the data recieved from the client.
				const size_t,	// the length of the data recieved from the client.
				connection * // this is a pointer to the memory that will store our data.
				);

/*	this is a big guy. 
	what this does is first urldecode the URL,
	and then reads the requested file into a buffer.
	at this point in time i do not know what will happen if
	the file cannot be read, either due to permissions or if it doesn't exist.. */

const int fulfill_request(connection *);

/*	this guy is a helper to fulfill_request,
	it simply reads the file into a fat buffer. */

const int get_that_file(connection *);

/*	this little guy is just here to clean up after all my malloc'ing.
	make sure this is called. it is responsible for EVERYTHING. */

void connection_destroy(connection *p);

/*	reads characters from src, decoding encoded valuesinto dest.
	might be an attack vector if the function wasn't written right. */

void url_decode(char *dest, const char *src);

/*	here is how we know what error happened!
	reads the status code, and then makes some text based on it.
	it kindly fills the data buffer, and the header for us. */

void error_code_to_data(http_data_out *);

/*	helper function to the above, it fills out the data/header
	based on the http code. */

void make_error_data(char *buf, http_data_out *out, http_status_code code, char *code1, char *code2);

/*	another function related to teh codez,
	 this one gives us a header string depending on the http code. */

const char *http_code_to_str(http_status_code x);

/*	this returns a mime type based on the resource given,
	by checking the file extension at the end.
	defaults to application/octet-stream */

const char *get_mime_type(char *resource);

#endif
