#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#define KILOBYTE 1024

typedef int sockfd;

typedef enum {
	GET = 500,
	POST,
	HEAD,
/* represends a bad request type */
	INVALID = 666
} http_request_type;

typedef enum {
	HTTP_OK = 200,
	HTTP_BAD_REQUEST = 400,
	HTTP_NOT_FOUND = 404,
	HTTP_URI_TOO_LONG = 414,
	HTTP_INTERNAL_ERROR = 500,
	HTTP_NOT_IMPLEMENTED = 501,
	HTTP_VERSION_UNSUPPORTED = 505
} http_status_code;

/* structure to hold the first line, i.e "GET /index.html HTTP/1.1" */
/* and hold the headers, etc */

typedef struct {
	http_request_type request_type;
	char *request_method;
	char *resource;
	char http_ver[20];
	char headers[1 * KILOBYTE];
} http_data_in;

/* structure to hold the response to the browser/client */

typedef struct {
	http_status_code status;
	char *header;
	unsigned long header_size;
	char *data;
	size_t content_size;
	char mimetype[100];
	int file;
	unsigned char sendfile;
} http_data_out;

/* structure to hold the socket data passed to the thread*/

typedef struct {
	/* the socket that the connection is on */
	sockfd socket;
	/* all the client data */
	char clientip[NI_MAXHOST];
	char clientport[NI_MAXSERV];
	struct sockaddr_storage client;
	socklen_t client_socksize;
	/* server data */
	char serverip[NI_MAXHOST];
	char port[NI_MAXSERV];
} connection_data;

/* structure to hold the entire http request AND response. \o/ */

typedef struct {
	http_data_in req;
	http_data_out response;
	connection_data *conn;
} connection;

/* structure for mimetypes */

typedef struct {
	char *ext;
	char *mimetype;
} MIMEtype;

#endif
