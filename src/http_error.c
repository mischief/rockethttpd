#include "http_error.h"

static const char *error_skel = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\r\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\r\n<title>%s</title>\r\n</head>\r\n<body>\r\n<h1>%s</h1><p>%s</p>\r\n</body>\r\n</html>\r\n";
extern const char *reply_header;

/* this function, along with the two following it, generate our error messages. yay! */

void error_code_to_data(http_data_out *out) {
	http_status_code code = out->status;
	char buf[2 * KILOBYTE] = {0};
	switch(code) {
		case HTTP_BAD_REQUEST:
			fill_error_template(buf, out, code, "400 Bad Request", "The request could not be understood by the server due to malformed syntax.");
			break;
		case HTTP_NOT_FOUND:
			fill_error_template(buf, out, code, "404 Not Found", "The server was unable to find the resource specified.");
			break;
		case HTTP_URI_TOO_LONG:
			fill_error_template(buf, out, code, "414 Request-URI Too Long", "The Reqest-URI is too long.");
			break;
		case HTTP_INTERNAL_ERROR:
			fill_error_template(buf, out, code, "500 Internal Server Error", "The server encountered an unexpected condition which prevented it from fulfilling the request.");
			break;
		case HTTP_NOT_IMPLEMENTED:
			fill_error_template(buf, out, code, "501 Not Implemented", "The server does not support the functionality required to fulfill the request.");
			break;
		case HTTP_VERSION_UNSUPPORTED:
			fill_error_template(buf, out, code, "505 HTTP Version Not Supported", "The server does not support the HTTP version requested.");
			break;
		case HTTP_OK:
			/* we shouldn't even get here... */
			break;
		default:
			break;
	}
}

void fill_error_template(char *buf, http_data_out *out, http_status_code code, char *code1, char *code2) {
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
