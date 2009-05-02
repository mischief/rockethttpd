#include "common_types.h"

/* ======================
 * 	templates for .c
 * ====================== */

const char *dir_page =
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
"<head>\n"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"
"<title>Index of %s</title>\n" /* directory we are indexing */
"</head>\n"
"<body>\n"
"\%s" /* dir_entry listing goes here */
"</body></html>\n";

const char *dir_entry = "";

/* ======================
 * 	templates for http.c
 * ====================== */

const char *error_skel = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\r\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\r\n<title>%s</title>\r\n</head>\r\n<body>\r\n<h1>%s</h1><p>%s</p>\r\n</body>\r\n</html>\r\n";

/*	see this for info on the reply skeleton
	http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html
*/

const char *reply_header =
"%s\r\n" /* reply code + string */
"Content-Type: %s\r\n"
"Connection: close\r\n"
"Content-Length: %lu\r\n" /* size of data after header */
"Server: %s\r\n\r\n"; /* PROGRAM " " VERSION */

/* set default mime-type here */
const char *fallback_mime = "application/octet-stream";

/* array of mime-type structs, see common_types.h */
const MIMEtype mime_types[] = {

	/* text */
	{	"html",	"text/html"						}, /* .html */
	{	"htm",	"text/html"						}, /* .htm */
	{	"css",	"text/css"						}, /* .css */
	{	"js",	"application/javascript"		}, /* .js */
	{	"xhtml","application/xhtml+xml"			}, /* .xhtml */
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
	/* other */
	{	"pdf",	"application/pdf"				}, /* .pdf */
	/* sentinel, do not remove */
	{0,0}
};
