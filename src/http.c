#include "http.h"

/*	see this for info on the reply skeleton
 * http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html */

const char *reply_header =
"%s\r\n" /* reply code + string */
"Content-Type: %s\r\n" /* content type? */
"Connection: close\r\n" /* maybe implement keep-alive later */
"Content-Length: %lu\r\n" /* size of data amfter header */
"Server: %s\r\n\r\n"; /* PROGRAM " " VERSION */

extern const char *error_skel;
extern const char *fallback_mime;
extern const MIMEtype mime_types[];

const int parse_http_request(char *data, const size_t len, connection *ret) {
	ret->response.status = HTTP_OK;

	char *tok, *state;
	char *subtok, *substate;

	char crlf[] = "\r\n"; /* this is how we break the request into useable pieces. :) */
	int line = 0;

	while( (tok = strtok_r(data, crlf, &state)) != NULL ) {
		if(line == 0) {
			/* parse the first line into subtokens: request method, resource, http ver */
			if( (subtok = strtok_r(tok, " ", &substate)) != NULL ) {
				/* duplicate string to data structure */
				ret->req.request_method = strdup(subtok);
				if(ret->req.request_method == NULL) {
					ret->response.status = HTTP_INTERNAL_ERROR;
					error_code_to_data( &(ret->response) );
					return -1;
				}
				/* okay, now figure out what method that was. */
				ret->req.request_type = get_request_type(subtok);

				/* okay, so if it isn't one of those, it's invalid. good day sir.*/
				if(ret->req.request_type == INVALID) {
					ret->response.status = HTTP_BAD_REQUEST;
					error_code_to_data( &(ret->response) );
					return -1;
				}
			}
			/* so apparently for these subsequent calls to strtok_r,
			the first param has to be null if i expect this to work right */

			/* okay, now parse the HTTP resource string. */
			if( (subtok = strtok_r(NULL, " ", &substate)) != NULL ) {
				if(strlen(subtok) > FILENAME_MAX) {
					/* alright, if you're submitting shit this big, gtfokthx. */
					ret->response.status = HTTP_URI_TOO_LONG;
					error_code_to_data( &(ret->response) );
					return -1;
				} else {
					/* copy the resource string to the struct */
					ret->req.resource = strdup(subtok);
					if(ret->req.resource == NULL) {
						ret->response.status = HTTP_INTERNAL_ERROR;
						error_code_to_data( &(ret->response) );
						return -1;
					}
				}
			}


			if( (subtok = strtok_r(NULL, " ", &substate)) != NULL ) {
				if(strlen(subtok) > 20) {
					/* okay, now the idiot is sending a HTTP version that is too big for our buffer. */
					ret->response.status = HTTP_VERSION_UNSUPPORTED;
					error_code_to_data( &(ret->response) );
					return -1;
				} else {
					strncpy(ret->req.http_ver, subtok, 20);
				}
			}
			line=1;
		} // end if for parsing http request line

		/* do header parsing here. */

		break;
	} // end of while which processes the http header
	return 0;
} // end of parse_http_request

const int fulfill_request(connection *ret) {
	http_status_code status = HTTP_OK; // eh?
	char buf[8 * KILOBYTE] = {0};
	char badstr[2][4] = { "../", "/" };

	/* important: check if it's something bad! */

	if( (strstr(buf, badstr[0]) != 0) || (strncmp(buf, badstr[1], 1) == 0) ) {
		/* d'oh! bad! */
		ret->response.status = HTTP_BAD_REQUEST;
		error_code_to_data( &(ret->response) );
		return -1;
	}

	/* a bit kludge-ish, fix this later */

	if( strcmp( &ret->req.resource[strlen(ret->req.resource)-1], "/") == 0) {
		/* we have a directory! */


		/* first check for an index page */
		if( exist_index(ret) ) {
			if( get_that_file(ret) != 0 ) return -1;
		} else {
			/* yay! directory listings work! */
			if( make_dir_list(ret) != 0 ) {
				ret->response.status = HTTP_NOT_FOUND;
				error_code_to_data( &(ret->response) );
				return -1;
			}
		}

	} else {
		/* we haz a file request. */
		if( get_that_file(ret) != 0 ) return -1;
	}

	/* okay, content is dealt with. now make a correct header to send back */
	sprintf(buf, reply_header, http_code_to_str(status), ret->response.mimetype, ret->response.content_size, PROGRAM " " VERSION);
	ret->response.header_size = strlen(buf);
	/* copy the buffer to the header output */
	ret->response.header = strdup(buf);
	if(ret->response.header == NULL) {
		ret->response.status = HTTP_INTERNAL_ERROR;
		error_code_to_data( &(ret->response) );
		return -1;
	}

	/* done, now all the data should be sent off. :) */
	return 0;
} // end of fulfill_request

const int get_that_file(connection *ret) {
	char buf[8 * KILOBYTE];
	memset(buf, 0, sizeof(buf));
	unsigned long bytes = 0;

	/* the if statement below is for filling a file request *only* */

	/* okay, this needs to change later. but this works for now. */
	/* strip the beginning / off the url by reading request+1 */
	/* urldecode the request into the temp buffer */
	url_decode(buf, ret->req.resource+1);

	/* grab ze mime type from ze struct */
	strcpy(ret->response.mimetype, get_mime_type(buf));
	/* okay, should be good to go. open it. :) */
	ret->response.file = fopen(buf, "r");
	if(ret->response.file == NULL) {
		/* yes, we have no bananas. */
		ret->response.status = HTTP_NOT_FOUND;
		error_code_to_data( &(ret->response) );
		return -1;
	}

	/* a little trickery from cplusplus.com to get the file size */
	fseek(ret->response.file, 0, SEEK_END);
	ret->response.content_size = ftell (ret->response.file);
	rewind(ret->response.file);
	/* get us a nice fat buffer for the file */
	ret->response.data = (char*) malloc(sizeof(char) * ret->response.content_size);
	if(ret->response.data == NULL) {
		/* d'oh, OOM! */
		BARK("couldn't malloc for file");
		ret->response.status = HTTP_INTERNAL_ERROR;
		error_code_to_data( &(ret->response) );
		return -1;
	}
	/* read the file :D */
	bytes = fread (ret->response.data , 1, ret->response.content_size, ret->response.file);
	/* okay, it might help if i closed the file */
	fclose(ret->response.file);
	if(bytes != ret->response.content_size) {
		/* read error. :( */
		BARK("couldn't read whole file");
		ret->response.status = HTTP_INTERNAL_ERROR;
		error_code_to_data( &(ret->response) );
		   return -1;
	}
	return 0;
} /* end of get_that_file */

void connection_destroy(connection *p) {
	/*	make sure to call this when we're done with the structure!	*/
	/*	here is where we deal with de-allocating that memory we got	*/

	/* first the request bits */
	if(p->req.request_method != NULL)
		free(p->req.request_method);

	if(p->req.resource != NULL)
		free(p->req.resource);

	/* now the reply bits */
	if(p->response.header != NULL)
		free(p->response.header);
	if(p->response.data != NULL)
		free(p->response.data);

	/* now the connection part */
	memset(p->conn, 0, sizeof(connection_data));
	free(p->conn);

	/* hit the big red button */
	memset(p, 0, sizeof(connection));
}
