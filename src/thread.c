#include "thread.h"
#include "common_types.h"
#include "networking.h"
#include "http.h"
#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>

void *dispatch_request(void *arg)
{
        connection c;
	memset(&c, 0, sizeof(connection));

	c.conn = (connection_data *) arg;

	/* bitch who are you. */
	int rv = getnameinfo((struct sockaddr *) &(c.conn->client), c.conn->client_socksize, c.conn->clientip, 
sizeof(c.conn->clientip), c.conn->clientport, sizeof(c.conn->clientport), NI_NUMERICHOST | NI_NUMERICSERV);
        if ( rv != 0) {
                fprintf(stderr, "[-] in file \"%s\", line %d: getnameinfo: error %d: %s\n", __FILE__, __LINE__, rv, gai_strerror(rv));
		//return EXIT_FAILURE;
        }

	print_con_dat(c.conn); printf("connection established\n");        
        
        int in = 0,	// bytes in
        out = 0,	// bytes out
        nbytes = 0;
        
	char buf[8 * KILOBYTE]; // hold our data temporarily
	memset(buf, 0, sizeof(buf));

	if((nbytes = read(c.conn->socket, buf, sizeof(buf) )) <= 0)
	{
		if(nbytes == 0)
		{
			// connection closed, what to do :(
		} else
		{
			// we have a problem :(
			fprintf(stderr, "[-] in file \"%s\", line %d: recv: %s\n", __FILE__, __LINE__, strerror(errno));
		}
	} else
	{
	// okay, got some data! figure out what the hell it was.
		in+=nbytes;

		parse_http_request(buf, in, &c);

		print_con_dat(c.conn); printf("recieved http request: %s %s %s\n", c.req.request_method, c.req.resource, c.req.http_ver);

		if(fulfill_request(&c) < 0) {
			/* if something does go bad it will probably show up here */
			/*
			fprintf(stderr, "[-] in file \"%s\", line %d: fulfill_request: something bad happened: %d\n", __FILE__, __LINE__, client_req.response.status);
			*/
		}
		
		/* send the header first*/
		nbytes = c.response.header_size;
		//printf("[+] %s <-> %s: sending %d bytes of header data\n", c.conn->serverip, c.conn->clientip, nbytes);
		if( sendall(c.conn->socket, c.response.header, &nbytes) < 0) {
			// got -1, error!
			fprintf(stderr, "[-] in file \"%s\", line %d: sendall: %s\n", __FILE__, __LINE__, strerror(errno));
		}
		out+=nbytes;

		/* send off the content, gogo! */
		nbytes = c.response.content_size;
		//printf("[+] %s <-> %s: sending %d bytes of html data\n", c.conn->serverip, c.conn->clientip, nbytes);
		if( sendall(c.conn->socket, c.response.data, &nbytes) < 0) {
                        // got -1, error!
                        fprintf(stderr, "[-] in file \"%s\", line %d: sendall: %s\n", __FILE__, __LINE__, strerror(errno));
		}
		out+=nbytes;

	}
        print_con_dat(c.conn); printf("received %d bytes. sent %d bytes.\n", in, out);

	// clean up!
        close(c.conn->socket);
	connection_destroy(&c);
	return (void *) 0;
}
