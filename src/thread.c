#include "thread.h"

/* rfc 822 conformant */
static const char *timefmt = "%a, %d %b %Y %T %z";

void *dispatch_request(void *arg) {
	connection c;

	/* our 'connection' needs to know who we are connected to */
	c.conn = (connection_data *) arg;

	/* bitch who are you. */
	int rv = getnameinfo((struct sockaddr *) &(c.conn->client), c.conn->client_socksize, c.conn->clientip,
		sizeof(c.conn->clientip), c.conn->clientport, sizeof(c.conn->clientport), NI_NUMERICHOST | NI_NUMERICSERV);
	if ( rv != 0) {
		ERROR("getnameinfo(): %s\n", gai_strerror(rv));
	}

	/* print_con_dat(c.conn); printf("connection established\n"); */

	int in = 0,	/* bytes in */
	out = 0,	/* bytes out */
	nbytes = 0;

	/* hold our data temporarily */
	char buf[8 * KILOBYTE] = {0};

	if((nbytes = read(c.conn->socket, buf, sizeof(buf) )) <= 0) {
		if(nbytes == 0) {
			/* connection closed, what to do? */
		} else {
			/* -1, we have a problem :( */
			ERROR("recv(): %s\n", strerror(errno));
		}
	} else {
		/* okay, got some data! figure out what the hell it was. */
		in+=nbytes;

		parse_http_request(buf, in, &c);

		/* print_con_dat(c.conn); printf("recieved http request: %s %s %s\n", c.req.request_method, c.req.resource, c.req.http_ver); */

		/* check if the connection is okay, if not,
		 * error data is already taken care of and
		 * can be sent off. */
		if(c.response.status == HTTP_OK) {
			if(fulfill_request(&c) < 0) {
				/* if something does go bad it will probably show up here */
			}
		}

		/* print_con_dat(c.conn); printf("sending response '%s'\n", http_code_to_str(c.response.status) ); */

		/* send the header first*/
		nbytes = c.response.header_size;
		if( sendall(c.conn->socket, c.response.header, &nbytes) < 0) {
			ERROR("sendall(): %s\n", strerror(errno));
		}
		out+=nbytes;

		/* send off the content, gogo! */
		if(c.response.sendfile > 0) {
			/* if we were instructed to send a file, use senfile(). */
			nbytes = sendfile(c.conn->socket, c.response.file, 0, c.response.content_size);
			if(nbytes <= 0) {
				ERROR("sendfile(): %s\n", strerror(errno));
			}
			if(nbytes != c.response.content_size) {
				ERROR("sendfile(): transfer size mismatch: size %zu - sent %d\n", c.response.content_size, nbytes);
			}
			close(c.response.file);
		} else {
			/* otherwise, send the data buffer. */
			nbytes = c.response.content_size;
			if( sendall(c.conn->socket, c.response.data, &nbytes) < 0) {
				ERROR("sendall: %s\n", strerror(errno));
			}
		}
		out+=nbytes;

	}

	/* get the current time */
	time_t sec;
	struct tm now;
	sec = time(NULL);
	strftime(buf, sizeof(buf), timefmt, localtime_r(&sec, &now));

	/* print out connection summary */
	printf("[%c] %s [%s] %d %dB \"%s\"\n", c.response.status == 200 ? 'o' : '~', c.conn->clientip, buf, c.response.status, nbytes, c.req.resource);

	fflush(NULL);
	/* clean up! */
	close(c.conn->socket);
	connection_destroy(&c);
	return (void *) 0;
}
