#include "http.h"

extern const char *error_skel;
extern const char *reply_header;
extern const char *fallback_mime;
extern const MIMEtype mime_types[];

const int parse_http_request(char *data, const size_t len, connection *ret) {

	char *tok, *state;
	char *subtok, *substate;

	char crlf[] = "\r\n"; // this is how we break the request into useable pieces. :)
	int line = 0;

	while( (tok = strtok_r(data, crlf, &state)) != NULL ) {
		if(line == 0) {
			/* parse the first line into subtokens: request method, resource, http ver */
			if( (subtok = strtok_r(tok, " ", &substate)) != NULL ) {
				/* copy the method string into the buffer, first allocating enough memory for it */
				ret->req.request_method = (char *) malloc(sizeof(char) * strlen(subtok));
				if(ret->req.request_method == NULL) {
		                        ret->response.status = HTTP_INTERNAL_ERROR;
                	        	error_code_to_data( &(ret->response) );
	                        	return -1;
				}
				strcpy(ret->req.request_method, subtok);
				/* okay, now figure out what method that was. */
				ret->req.request_type = get_request_type(subtok);

				/* okay, so if it isn't one of those, it's invalid. good day sir.*/
				if(ret->req.request_type == INVALID) {
					ret->response.status = HTTP_BAD_REQUEST;
					return -1;
				}
			}
			/* so apparently for these subsequent calls to strtok_r,
			the first param has to be null if i expect this to work right */

			/* okay, now parse the HTTP resource string. */
			if( (subtok = strtok_r(NULL, " ", &substate)) != NULL ) { // next the file
				if(strlen(subtok) > FILENAME_MAX) { // this is the size of the resource requested.
					/* alright, if you're submitting shit this big, gtfokthx. */
					ret->req.request_type = INVALID;
					ret->response.status = HTTP_URI_TOO_LONG;
					return -1;
				} else {
					/* copy the resource string to the struct */
					strncpy(ret->req.resource, subtok, 1 * KILOBYTE); /* only copy 1kb for now. :> */
				}
			}


			if( (subtok = strtok_r(NULL, " ", &substate)) != NULL ) {
				if(strlen(subtok) > 20) {
					/* okay, now the idiot is sending a HTTP version that is too big for our buffer. */
                                        ret->req.request_type = INVALID;
					ret->response.status = HTTP_VERSION_UNSUPPORTED;
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
		getcwd(buf, 8 * KILOBYTE);
		BARK; printf("directory was requested: \"%s\"\n", buf);
		memset(buf, 0, sizeof(buf));

		/* oh, bother. no function to support directory lists now, 404 :D */

		/* ========================
		DIR FUNCTIONALITY GOES HERE
		======================== */

		ret->response.status = HTTP_NOT_FOUND;
		error_code_to_data( &(ret->response) );
		return -1;
	} else {
		/* we haz a file request. */
		if( get_that_file(ret) != 0 ) return -1;
	}

	/* okay, content is dealt with. now make a correct header to send back */

	/* shitty attempt at a half-decent header */
	sprintf(buf, reply_header, http_code_to_str(status), ret->response.mimetype, ret->response.content_size, PROGRAM " " VERSION);
	ret->response.header_size = strlen(buf);
	ret->response.header = (char *) malloc(sizeof(char) * ret->response.header_size);
	if(ret->response.header == NULL) {
		ret->response.status = HTTP_INTERNAL_ERROR;
		error_code_to_data( &(ret->response) );
		return -1;
	}

	strcpy(ret->response.header, buf);

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
	// better go to the beginning!
	rewind(ret->response.file);
	/* get us a nice fat buffer for the file */
	ret->response.data = (char*) malloc(sizeof(char) * ret->response.content_size);
	if(ret->response.data == NULL) {
		/* d'oh, OOM! */
		BARK;
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
		BARK;
		ret->response.status = HTTP_INTERNAL_ERROR;
		error_code_to_data( &(ret->response) );
		   return -1;
	}
	char *dir = get_current_dir_name();
	strcpy(buf, dir);
	free(dir);
	print_con_dat(ret->conn); printf("read %lu bytes from file \"%s/%s\"\n", bytes, buf, ret->req.resource+1);
	return 0;
} // end of get_that_file

void connection_destroy(connection *p) {
	/*	make sure to call this when we're done with the structure!	*/
	/*	here is where we deal with de-allocating that memory we got	*/

	/* first the request bits */
	if(p->req.request_method != NULL)
		free(p->req.request_method);

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
