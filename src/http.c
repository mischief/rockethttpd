#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "common_types.h"
#include "networking.h"
#include "http.h"
#include "defs.h"

const char error_skel[] = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\r\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\r\n<title>%s</title>\r\n</head>\r\n<body>\r\n<h1>%s</h1><p>%s</p>\r\n</body>\r\n</html>\r\n";

/*	see this for info on the reply skeleton
	http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html
*/

const char reply_header[] =   "%s\r\n" // Status-Line
			"Content-Type: %s\r\n"
                        "Connection: close\r\n"
                        "Content-Length: %lu\r\n" // ret->response.content_size
                        "Server: %s\r\n\r\n"; // VERSION


/* set default mime-type here */
const char *fallback_mime = "application/octet-stream";

/* array of mime-type structs, see common_types.h */
const MIMEtype mime_types[] = {

	/* text */
	{	"html",	"text/html"			}, /* .html */
	{	"htm",	"text/html"			}, /* .htm */
	{	"css",	"text/css"			}, /* .css */
	{	"js",	"application/javascript"	}, /* .js */
	{	"xhtml","application/xhtml+xml"		}, /* .xhtml */
	/* images */
	{	"gif",	"image/gif"			}, /* .gif */
	{	"jpg",	"image/jpeg"			}, /* .jpg */
	{	"jpeg",	"image/jpeg"			}, /* .jpeg */
	{	"png",	"image/png"			}, /* .png */
	{	"ico",	"image/x-icon"			}, /* .ico */
	/* audio */
	{	"mp3", 	"audio/mpeg"			}, /* .mp3 */
	{	"ogg",	"application/ogg"		}, /* .ogg */
	/* archives */
	{	"7z",	"application/x-7z-compressed"	}, /* .7z */
	{	"rar",	"application/x-rar-compressed"	}, /* .rar */
	{	"bz2",	"application/x-bzip"		}, /* .bz2 */
	{	"zip",	"application/zip"		}, /* .zip */
	{	"gz",	"application/x-gzip"		}, /* .gz */
	{	"tar",	"application/x-tar"		}, /* .tar */
	/* other */
	{	"pdf",	"application/pdf"		}, /* .pdf */
	/* sentinel, do not remove */
	{0,0}
};


const int parse_http_request(char *data, const size_t len, connection *ret)
{
	//char buf[8 * KILOBYTE];
	//memset(buf, 0, sizeof(buf));
	//memset(ret, 0, sizeof(connection_data));

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
                                if( !strcmp(subtok, "GET") || !strcmp(subtok, "get") )  {
                                        ret->req.request_type = GET;
                                } else if( !strcmp(subtok, "POST") || !strcmp(subtok, "post") ) {
                                        ret->req.request_type = POST;
				} else if( !strcmp(subtok, "HEAD") || !strcmp(subtok, "head") ) {
					ret->req.request_type = HEAD;
				} else {
					/* okay, so if it isn't one of those, it's invalid. good day sir.*/
					ret->req.request_type = INVALID;
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

			if( (subtok = strtok_r(NULL, " ", &substate)) != NULL ) { // http version!
				if(strlen(subtok) > 20) {
					/* okay, not the idiot is sending a HTTP version that is too big for our buffer. */
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
	char buf[8 * KILOBYTE];
	memset(buf, 0, sizeof(buf));
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
	sprintf(buf, reply_header, http_code_to_str(status), ret->response.mimetype, ret->response.content_size, VERSION);
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
	
void url_decode(char *dest, const char *src) {
	/*	assume that dest is at least as large as src.
		if the guy on the other end can't do at least that. plzdie. */
	const char *p = src;
	char code[3] = {0};
	unsigned long ascii = 0;
	char *end = NULL;

	while(*p) {
			// example: %20
		if(*p == '%') {
			// copy everything after % to code, which is now "20\0"
			memcpy(code, ++p, 2);
			// convert string to unsigned long, in base 16 (hex).
			ascii = strtoul(code, &end, 16);
			// now put that char in dest.
			*dest = (char)ascii;
			// increase dest one 
			dest++;
			// and increase p 2, since we read 2 chars for the hex code
			p += 2;
		} else {
			// if we don't have a % character, just copy the character and advance the pointers.
			*dest++ = *p++;
		}
	}
	//  this should cause the end of dest to be a null character..
	dest = 0;
}
/* this function, along with the two following it, generate our error messages. yay! */

void error_code_to_data(http_data_out *out) {
	http_status_code code = out->status;
	char buf[2 * KILOBYTE];
	switch(code) {
        case HTTP_BAD_REQUEST:
		make_error_data(buf, out, code, "400 Bad Request", "The request could not be understood by the server due to malformed syntax.");
		break;
        case HTTP_NOT_FOUND:
		make_error_data(buf, out, code, "404 Not Found", "The server was unable to find the resource specified.");
                break;
        case HTTP_URI_TOO_LONG:
		make_error_data(buf, out, code, "414 Request-URI Too Long", "The Reqest-URI is too long.");
                break;
        case HTTP_INTERNAL_ERROR:
		make_error_data(buf, out, code, "500 Internal Server Error", "The server encountered an unexpected condition which prevented it from fulfilling the request.");
                break;
        case HTTP_NOT_IMPLEMENTED:
		make_error_data(buf, out, code, "501 Not Implemented", "The server does not support the functionality required to fulfill the request.");
                break;
        case HTTP_VERSION_UNSUPPORTED:
                make_error_data(buf, out, code, "505 HTTP Version Not Supported", "The server does not support the HTTP version requested.");
                break;
	case HTTP_OK:
		// we shouldn't even get here...
		break;
	default:
		break;
        }
}

void make_error_data(char *buf, http_data_out *out, http_status_code code, char *code1, char *code2) {
	/* first get the error content */
	sprintf(buf, error_skel, code1, code1, code2);
	out->data = (char *) malloc(strlen(buf)+1);
	strcpy(out->data, buf);
	out->content_size = strlen(out->data);

	/* now make a nice header */
	/* the error message has hard-coded mime type of "text/html" */
	sprintf(buf, reply_header, http_code_to_str(code), "text/html", out->content_size, VERSION);
	/* size of the header code + 1 for null */
	out->header = (char *) malloc(strlen(buf)+1);
	strcpy(out->header, buf);
	out->header_size = strlen(out->header);
}

const char *http_code_to_str(http_status_code x) {
	// turn our status code into a string so that the browser knows what is going on	
	switch(x) {
	case HTTP_OK:
		return "HTTP/1.1 200 OK";
		break;
	case HTTP_BAD_REQUEST:
		return "HTTP/1.1 400 Bad Request";
		break;
	case HTTP_NOT_FOUND:
		return "HTTP/1.1 404 Not Found";
		break;
	case HTTP_URI_TOO_LONG:
		return "HTTP/1.1 414 Request-URI Too Long";
		break;
	case HTTP_INTERNAL_ERROR:
		return "HTTP/1.1 500 Internal Server Error";
		break;
	case HTTP_NOT_IMPLEMENTED:
		return "HTTP/1.1 501 Not Implemented";
		break;
	case HTTP_VERSION_UNSUPPORTED:
		return "HTTP/1.1 505 HTTP Version Not Supported";
		break;
	default:
		return "HTTP/1.1 200 OK";
		break;
	}
}

/* this function goes ahead and figures out a MIME 
type from the resource that the guy requested.
maybe this should be changed so it checks against the actual file,
not what the user requested. */

const char *get_mime_type(char *resource) {
	int i, len_mime, len = strlen(resource);
	
	/* go through our fat struct array looking for a mime type
	until we hit the sentinel. */
	for(i=0;mime_types[i].ext != 0;i++) {
		len_mime = strlen(mime_types[i].ext);
		if( strncmp(&resource[len-len_mime], mime_types[i].ext, len_mime) == 0) {
			return mime_types[i].mimetype;
		}
	}

	/* okay, no dice there. send the default. */
	return fallback_mime;
}
