#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include "defs.h"
#include <string.h>
#include <stdlib.h>

#include <strings.h>

#include "common_types.h"

/* get the method type of the HTTP request */
const http_request_type get_request_type(const char *str);

/* this returns a mime type based on the resource given,
 * by checking the file extension at the end.
 * defaults to application/octet-stream */

const char *get_mime_type(const char *resource);

/*	reads characters from src, decoding encoded valuesinto dest.
	might be an attack vector if the function wasn't written right. */

void url_decode(char *dest, const char *src);

/*	another function related to teh codez,
	 this one gives us a header string depending on the http code. */

const char *http_code_to_str(const http_status_code x);

#endif
