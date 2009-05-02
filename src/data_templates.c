/*	see this for info on the reply skeleton
	http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html
*/

const char *reply_header =
"%s\r\n" /* reply code + string */
"Content-Type: %s\r\n"
"Connection: close\r\n"
"Content-Length: %lu\r\n" /* size of data after header */
"Server: %s\r\n\r\n"; /* PROGRAM " " VERSION */
