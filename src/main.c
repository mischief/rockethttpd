#include "defs.h"
#include "thread.h"
#include "common_types.h"
#include "networking.h"

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

void usage(char *prog) {
	printf(	"[-] usage:\n"
		"[-]	%s [port] [webroot]\n"
		"[-]	%s -?		this help\n", prog, prog);
	exit(1);
}

int main(int argc, char **argv) {
	printf("[+] %s built on %s %s\n", VERSTRING, __DATE__, __TIME__);

/* this #ifdef block is to set up windows-only socket related matter,
 * and registering WSACleanup() with atexit() so what we know it is
 * called when any call to exit() is made. */

#ifdef WIN32
	WSADATA wsadat;
	WSAStartup(MAKEWORD(2,2),&wsadat);
	if( !atexit(WSACleanup()) ) {
		BARK("Registering WSACleanup() with atexit() failed\n");
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

	memset(servip, 0, INET6_ADDRSTRLEN);
	memset(servport, 0, sizeof(servport));

	if(chdir(argv[2]) == -1) {
		BARK("chdir('%s') failed: %s\n", argv[2], strerror(errno));
		exit(1);
	} else {
		printf("[+] chdir() to %s\n", argv[2]);
	}

	sockfd serv_sock = {0}; // server socket

	struct addrinfo hints, *servinfo;

	/* SO_REUSEADDR on. */
	int yes=1;
	/* getaddrinfo return value */
	int rv = 0;

	memset(&hints, 0, sizeof(hints));
	// set up hints struct for getaddrinfo()
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	rv = getaddrinfo(NULL, port, &hints, &servinfo);
	if (rv != 0) {
		BARK("getaddrinfo(): %s\n", gai_strerror(rv));
		return EXIT_FAILURE;
	}

	struct addrinfo *addr;
	for(addr = servinfo; addr != NULL; addr = addr->ai_next) {
		// fetch ourselves a socket file descriptor.
		if ((serv_sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) == -1) {
			BARK("socket(): %s\n", strerror(errno));
			continue;
		}

		/*	this turns SO_REUSEADDR on. what this does is allow the
			program to re-use the socket even if it is closed unexpectedly.
		*/
		if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			BARK("setsockopt(): %s\n", strerror(errno));
			exit(1);
		}

		if (bind(serv_sock, addr->ai_addr, addr->ai_addrlen) == -1) {
			close(serv_sock);
			BARK("bind(): %s\n", strerror(errno));
			continue;
		}

		break;

	}

	if (addr == NULL) {
		BARK("failed to bind\n");
		return EXIT_FAILURE;
	}

	/* figure out who we are */
	rv = getnameinfo(addr->ai_addr, addr->ai_addrlen, servip, sizeof(servip), servport, sizeof(servport), NI_NUMERICHOST | NI_NUMERICSERV);
	if (rv != 0) {
		BARK("getnameinfo(): %s\n", gai_strerror(rv));
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

	if (listen(serv_sock, BACKLOG) == -1) {
		BARK("listen(): %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	printf("[+] %s: waiting for connections on %s:%s\n", PROGRAM, servip, port);

	/* main server loop */
	while(1) {
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

		if (p->socket == -1) {
			free(p);
			BARK("accept(): %s\n", strerror(errno));
		} else {
			/* do the real bsns */
			if(pthread_create(&client_thr, &attr, dispatch_request, (void *) p) != 0) {
				/* couldn't create a thread :( */
				free(p);
				BARK("pthread_create(): %s\n", strerror(errno));
			}
		}

		/* flush all streams. */
		fflush(NULL);
	}

	return EXIT_SUCCESS;
}
