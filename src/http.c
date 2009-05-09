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

	char crlf[] = "\r\n";
	int line = 0;

	while( (tok = strtok_r(data, crlf, &state)) != NULL ) {
		if(line == 0) {
			// parse the first line into subtokens: request method, resource, http ver
			if( (subtok = strtok_r(tok, " ", &substate)) != NULL ) {
				// duplicate string to data structure
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
					ret->response.status = HTTP_NOT_IMPLEMENTED;
					error_code_to_data( &(ret->response) );
					return -1;
				}
			}

			// parse HTTP resource string.
			if( (subtok = strtok_r(NULL, " ", &substate)) != NULL ) {
				if(strlen(subtok) > FILENAME_MAX) {
					ret->response.status = HTTP_URI_TOO_LONG;
					error_code_to_data( &(ret->response) );
					return -1;
				} else {
					// copy resource string
					ret->req.resource = strndup(subtok, 7 * KILOBYTE);
					if(ret->req.resource == NULL) {
						ret->response.status = HTTP_INTERNAL_ERROR;
						error_code_to_data( &(ret->response) );
						return -1;
					}
				}
			}


			if( (subtok = strtok_r(NULL, " ", &substate)) != NULL ) {
				strncpy(ret->req.http_ver, subtok, 20);
			}
			line=1;
		} // end if for parsing http request line

		// do header parsing here.

		break;
	} // end of while which processes the http header
	return 0;
} // end of parse_http_request

const int fulfill_request(connection *ret) {
	http_status_code status = HTTP_OK;

	/* a bit kludge-ish, fix this later */

	if( strcmp( &ret->req.resource[strlen(ret->req.resource)-1], "/") == 0) {
		// first check for an index page
		if( exist_index(ret) ) {
			if( get_that_file(ret) != 0 ) return -1;
		} else {
			if( make_dir_list(ret) != 0 ) {
				ret->response.status = HTTP_NOT_FOUND;
				error_code_to_data( &(ret->response) );
				return -1;
			}
		}

	} else {
		// nope, file request.
		if( get_that_file(ret) != 0 ) return -1;
	}

	// create a header
	ret->response.header_size = asprintf(&ret->response.header, reply_header, http_code_to_str(status), ret->response.mimetype, ret->response.content_size, PROGRAM " " VERSION);
	if(ret->response.header_size <= 0 ) {
		ret->response.status = HTTP_INTERNAL_ERROR;
		error_code_to_data( &(ret->response) );
		return -1;
	}

	return 0;
} // end of fulfill_request

const int get_that_file(connection *ret) {
	char buf[8 * KILOBYTE];
	memset(buf, 0, sizeof(buf));

	// strip beginning / off
	url_decode(buf, ret->req.resource+1);

	// get mime type
	strcpy(ret->response.mimetype, get_mime_type(buf));

	ret->response.file = open(buf, O_RDONLY);
	if(ret->response.file < 0) {
		ret->response.status = HTTP_NOT_FOUND;
		error_code_to_data( &(ret->response) );
		return -1;
	}

	/* go to the end, get the file size as well */
	ret->response.content_size = lseek(ret->response.file, 0, SEEK_END);
	/* go to the beginning */
	lseek(ret->response.file, 0, SEEK_SET);

	/* make sure the code later on knows we want to send a file and not the data buffer */
	ret->response.sendfile = 0x1;

	return 0;
} /* end of get_that_file */

void connection_destroy(connection *p) {
	/*	make sure to call this when we're done with the structure!
	 * the purpose of this function is to deallocate any memory
	 * we have allocated but not freed. */

	// first the request
	if(p->req.request_method != NULL)
		free(p->req.request_method);
	if(p->req.resource != NULL)
		free(p->req.resource);

	// now the reply bits
	if(p->response.header != NULL)
		free(p->response.header);
	if(p->response.data != NULL)
		free(p->response.data);

	// now the connection
	free(p->conn);
}
