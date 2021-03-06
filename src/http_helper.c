#include "http_helper.h"

static const struct {
	char *method;
	http_request_type type;
} methods[] = {
	{ "GET", GET },
	/*{ "POST", POST }, */
	/*{ "HEAD", HEAD }, */
	{0, INVALID }
};

/* default content-type */
static const char *fallback_mime = "application/octet-stream";

/* array of mime-type structs, see common_types.h */
static const MIMEtype mime_types[] = {

	/* text */
	{	"html",	"text/html"						}, /* .html */
	{	"htm",	"text/html"						}, /* .htm */
	{	"css",	"text/css"						}, /* .css */
	{	"js",	"application/javascript"		}, /* .js */
	{	"xhtml","application/xhtml+xml"			}, /* .xhtml */
	{	"c",	"text/plain"					}, /* .c */
	{	"h",	"text/plain"					}, /* .h */
	{	"cc",	"text/plain"					}, /* .cc */
	{	"cpp",	"text/plain"					}, /* .cpp */
	{	"hpp",	"text/plain"					}, /* .hpp */
	/* images */
	{	"gif",	"image/gif"						}, /* .gif */
	{	"jpg",	"image/jpeg"					}, /* .jpg */
	{	"jpeg",	"image/jpeg"					}, /* .jpeg */
	{	"png",	"image/png"						}, /* .png */
	{	"ico",	"image/x-icon"					}, /* .ico */
	/* audio */
	{	"mp3", 	"audio/mpeg"					}, /* .mp3 */
	{	"ogg",	"application/ogg"				}, /* .ogg */
	/* archives */
	{	"7z",	"application/x-7z-compressed"	}, /* .7z */
	{	"rar",	"application/x-rar-compressed"	}, /* .rar */
	{	"bz2",	"application/x-bzip"			}, /* .bz2 */
	{	"zip",	"application/zip"				}, /* .zip */
	{	"gz",	"application/x-gzip"			}, /* .gz */
	{	"tar",	"application/x-tar"				}, /* .tar */
	{	"tgz",	"application/x-compressed"		}, /* .tgz */
	/* other */
	{	"pdf",	"application/pdf"				}, /* .pdf */
	{	"exe",	"application/octet-stream"		}, /* .exe */
	/* sentinel, do not remove */
	{0,0}
};

http_request_type get_request_type(const char *str) {
	int i = 0;
	for(i = 0; methods[i].method != NULL; ++i) {
		if( !strncasecmp(str, methods[i].method, 10) )  {
			return methods[i].type;
		}
	}

	/* if the for loop did not find it, then it is invalid. :( */
	return INVALID;
}

const char *get_mime_type(const char *resource) {
	int i, len_mime, len = strlen(resource);

	/* find a suitable content-type */
	for(i=0;mime_types[i].ext != 0;i++) {
		len_mime = strlen(mime_types[i].ext);
		if( strncmp(&resource[len-len_mime], mime_types[i].ext, len_mime) == 0) {
			return mime_types[i].mimetype;
		}
	}

	/* send default content-type */
	return fallback_mime;
}

void url_decode(char *dest, const char *src) {
	/* assume that dest is at least as large as src.
	 * if the guy on the other end can't do at least that. plzdie. */
	const char *p = src;
	char code[3] = {0};
	unsigned long ascii = 0;
	char *end = NULL;

	while(*p) {
		if(*p == '%') {
			memcpy(code, ++p, 2);
			ascii = strtoul(code, &end, 16);
			*dest = (char)ascii;
			dest++;
			p += 2;
		} else {
			*dest++ = *p++;
		}
	}
	/* set the end to \0 */
	dest = 0;
}

const char *http_code_to_str(const http_status_code x) {
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

char to_hex(char code) {
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str) {
	char *p = str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
	while (*p) {
		if (isalnum(*p) || *p == '-' || *p == '_' || *p == '.' || *p == '~' || *p == '/')
			*pbuf++ = *p;
//		else if (*p == ' ')
//			*pbuf++ = '+';
		else
			*pbuf++ = '%', *pbuf++ = to_hex(*p >> 4), *pbuf++ = to_hex(*p & 15);
		p++;
	}
	*pbuf = '\0';
	return buf;
}



