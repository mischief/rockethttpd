#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/* networking headers */
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/* threading header */
#include <pthread.h>

#include "thread.h"
#include "common_types.h"
#include "networking.h"
#include "defs.h"

void usage(char *prog)
{
	printf(	"[-] usage:\n"
		"[-]	%s [port] [webroot]\n"
		"[-]	%s -?		this help\n", prog, prog);
	exit(1);
}

int main(int argc, char **argv)
{
	printf("[+] %s, built on %s %s\n", VERSION, __DATE__, __TIME__);

/*	this #ifdef block is to set up windows-only socket related matter,
	and registering WSACleanup() with atexit() so what we know it is
	called when any call to exit() is made.
*/

#ifdef WIN32
	WSADATA wsadat;
	WSAStartup(MAKEWORD(2,2),&wsadat);
	if( !atexit(WSACleanup()) )
	{
		fprintf(stderr, "[-] in file \"%s\", line %d: Registering WSACleanup() with atexit() failed\n", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}
#endif

	/* maybe implement daemon(0, 0); system call later */

	if( argc != 3 || !strcmp(argv[1], "-?") ) {
		/* doesn't return */
		usage(argv[0]);
	}
	
	char *port = argv[1];
	char servip[INET6_ADDRSTRLEN];
	char servport[20];

	if(chdir(argv[2]) == -1) {
		fprintf(stderr, "[-] in file \"%s\", line %d: chdir( \"%s\" ): %s\n", __FILE__, __LINE__, argv[2], strerror(errno));
		exit(5);
	} else {
		//printf("[+] chdir() to %s\n", argv[2]);
	}

	sockfd serv_sock = {0}; // server socket

	struct addrinfo hints, *servinfo;

	int yes=1; // for the call to setsockopt(), to turn SO_REUSEADDR on.

	int rv = 0;; // used for getaddrinfo return value

	// set up hints struct for getaddrinfo()
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	// collect our information about what to bind on etc

	rv = getaddrinfo(NULL, port, &hints, &servinfo);
	if (rv != 0) {
		fprintf(stderr, "[-] in file \"%s\", line %d: getaddrinfo: %s\n", __FILE__, __LINE__, gai_strerror(rv));
		return EXIT_FAILURE;
	}

	// completely stolen from beej, loop through the results and bind on the first possible address
	
	struct addrinfo *addr = {0}; // temporary, used for this loop only
	for(addr = servinfo; addr != NULL; addr = addr->ai_next)
	{
		// fetch ourselves a socket file descriptor.
		if ((serv_sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) == -1)
		{
			fprintf(stderr, "[-] in file \"%s\", line %d: socket: %s\n", __FILE__, __LINE__, strerror(errno));
			continue;
		}

		/*	this turns SO_REUSEADDR on. what this does is allow the
			program to re-use the socket even if it is closed unexpectedly.
		*/
		if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			fprintf(stderr, "[-] in file \"%s\", line %d: setsockopt: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}

		if (bind(serv_sock, addr->ai_addr, addr->ai_addrlen) == -1)
		{
			close(serv_sock);
			fprintf(stderr, "[-] in file \"%s\", line %d: bind: %s\n", __FILE__, __LINE__, strerror(errno));
			continue;
		}

		break;

	}

	/* figure out who we are */
		
	if ( (rv = getnameinfo(addr->ai_addr, addr->ai_addrlen, servip, sizeof(servip), servport, sizeof(servport), 0)) != 0) {
		fprintf(stderr, "[-] in file \"%s\", line %d: getnameinfo: %s\n", __FILE__, __LINE__, gai_strerror(rv));
		return EXIT_FAILURE;
	}

	if (addr == NULL)
	{
		fprintf(stderr, "[-] in file \"%s\", line %d: failed to bind\n", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	/* all done with this structure */
	freeaddrinfo(servinfo);

	/* set up our threading details */

	pthread_attr_t attr;
	/* initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_t client_thr;

	/* now set up the listening socket */

	if (listen(serv_sock, BACKLOG) == -1)
	{
		fprintf(stderr, "[-] in file \"%s\", line %d: listen: %s\n", __FILE__, __LINE__, strerror(errno));
		return EXIT_FAILURE;
	}
	
	printf("[+] %s: waiting for connections on %s:%s\n", VERSION, servip, port);

	/* main server loop */
	while(1)
	{
		sockfd x;
		struct sockaddr_storage client;
		socklen_t client_socksize;
		client_socksize = sizeof(client);

		x = accept(serv_sock, (struct sockaddr *)&client, &client_socksize);

		connection_data *p = (connection_data *) malloc( sizeof(connection_data));
		memset(p, 0, sizeof(connection_data));
		/* where we store our client stuff, free() it inside the thread we pass it to */

		p->socket = x;
		memcpy(&p->client, &client, client_socksize);
		p->client_socksize = client_socksize;
		strcpy(p->port, port);
		strcpy(p->serverip, servip);

		if (p->socket == -1)
		{
			free(p);
			fprintf(stderr, "[-] in file \"%s\", line %d: accept: %s\n", __FILE__, __LINE__, strerror(errno));
		} else {
			/* do the real bsns */
			if(pthread_create(&client_thr, &attr, dispatch_request, (void *) p) != 0)
			{
				/* couldn't create a thread :( */
				free(p);
				fprintf(stderr, "[-] in file \"%s\", line %d: pthread_create: %s\n", __FILE__, __LINE__, strerror(errno));
			}
		}

		/* flush all streams. */
		fflush(NULL);
	}

	return EXIT_SUCCESS;
}
