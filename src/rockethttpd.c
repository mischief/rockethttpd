/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include "rockethttpd.h"

#define opt_in(i,s1,s2) (!strcmp(argv[i],s1) || !strcmp(argv[i],s2))

static int plzdie = 0;

void usage(char *prog);
void exiting();
void die (int sig);
void mainloop();

int main(int argc, char **argv) {
	INFO("%s built on %s %s\n", VERSTRING, __DATE__, __TIME__);

	char root[PATH_MAX];
	char bindaddr[INET6_ADDRSTRLEN];
	char port[5];

	char servip[INET6_ADDRSTRLEN];
	char servport[20];

	int i;

	size_t threadcount = 0;

	char pidfile[FILENAME_MAX];
	char configfile[PATH_MAX] = {0};
	char files[2][PATH_MAX] = { "/etc/rockethttpd.conf", "./rockethttpd.conf" };

	for(i = 1; i < argc; ++i) {
		if( opt_in(i, "-h", "--help") ) {
	    	usage(argv[0]);
	    	return 1;

		} else if( opt_in(i, "-c", "--config") ) {
			if (++i < argc) {
				if (strlen(argv[i]) < sizeof(files[0]) ) strcpy(configfile, argv[i]);
			} else {
				FATAL("no config file specified\n");
				return 1;
			}

		} else {
	    	FATAL("invalid argument: '%s'\n", argv[i]);
	    	return 1;

		}
	}

	atexit(exiting);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, die);
	siginterrupt(SIGINT, 1);

	config_item conf[] = {
		{ "PORT", &port, 5, 's' },
		{ "BINDADDR", &bindaddr, INET6_ADDRSTRLEN, 's' },
		{ "WWWROOT", &root, PATH_MAX, 's' },
		{ "PIDFILE", &pidfile, FILENAME_MAX, 's' },
		{ "THREADS", &threadcount, 0, 'z' },
		{0}
	};

	if(configfile[0] != 0) {
		if( parse_config_file(configfile, conf) == 0 ) {
			INFO("read configuration from file: '%s'\n", configfile);
		} else {
			ERROR("could not parse config file '%s'\n", configfile);
			FATAL("could not parse any config files. aborting.\n");
			return 1;
		}
	} else {

		for(i = 0; ; ++i) {
			if( parse_config_file(files[i], conf) == 0 ) {
				INFO("read configuration from file: '%s'\n", files[i]);
				break;
			} else {
				ERROR("could not parse config file '%s'\n", files[i]);
			}

			/* if we have processed all the files ... */
			if(i == sizeof(files)/sizeof(files[0]) ) {
				FATAL("could not parse any config files. aborting.\n");
				return 1;
			}
		}
	}

	memset(servip, 0, INET6_ADDRSTRLEN);
	memset(servport, 0, sizeof(servport));

	if(chdir(root) == -1) {
		FATAL("chdir('%s') failed: %s\n", root, strerror(errno));
		return 1;
	} else {
		INFO("chdir() to %s\n", root);
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

	/* set up our listening socket */
	if (getaddrinfo(NULL, port, &hints, &servinfo) != 0) {
		FATAL("getaddrinfo(): %s\n", gai_strerror(rv));
		return 1;
	}

	serv_sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (serv_sock == -1) {
		ERROR("socket(): %s\n", strerror(errno));
		return 1;
	}

	/*	this turns SO_REUSEADDR on. what this does is allow the
	 * program to re-use the socket even if it is closed unexpectedly. */
	if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		freeaddrinfo(servinfo);
		close(serv_sock);
		FATAL("setsockopt(): %s\n", strerror(errno));
		return 1;
	}

	if (bind(serv_sock, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		freeaddrinfo(servinfo);
		close(serv_sock);
		ERROR("bind(): %s\n", strerror(errno));
		return 1;
	}


	/* figure out who we are */
	rv = getnameinfo(servinfo->ai_addr, servinfo->ai_addrlen, servip, sizeof(servip), servport, sizeof(servport), NI_NUMERICHOST | NI_NUMERICSERV);
	if (rv != 0) {
		freeaddrinfo(servinfo);
		close(serv_sock);
		FATAL("getnameinfo(): %s\n", gai_strerror(rv));
		return 1;
	}

	/* all done with this structure */
	freeaddrinfo(servinfo);

	/* set up threading */

	size_t stacksize = 204800;
	pthread_attr_t attr;
	/* initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&attr, stacksize);

	pthread_t client_thr;

	int cork_on = 1, sockopterr;

	/* now set up the listening socket */

	if (listen(serv_sock, BACKLOG) == -1) {
		FATAL("listen(): %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	INFO("waiting for connections on %s:%s\n", servip, port);

	/* main server loop */

	while(!plzdie) {
		sockfd x;
		struct sockaddr_storage client;
		socklen_t client_socksize;
		client_socksize = sizeof(client);

		x = accept(serv_sock, (struct sockaddr *)&client, &client_socksize);

		if(x > -1) {

			sockopterr = setsockopt(x, IPPROTO_TCP, TCP_CORK, &cork_on, sizeof(cork_on));
			if(sockopterr < 0)
				ERROR("setsockopt(): %s\n", strerror(errno));

			connection_data *p = malloc(sizeof(*p));

			/* where we store our client stuff, free() it inside the thread we pass it to */
			p->socket = x;
			memcpy(&p->client, &client, client_socksize);
			p->client_socksize = client_socksize;
			strcpy(p->port, port);
			strcpy(p->serverip, servip);

			/* do the real bsns */
			if(pthread_create(&client_thr, &attr, dispatch_request, (void *) p) != 0) {
				/* couldn't create a thread :( */
				free(p);
				close(x);
				ERROR("pthread_create(): %s\n", strerror(errno));
				return 1;
			}

		} else if(errno != EINTR) {
			ERROR("accept(): %s\n", strerror(errno));
		}
	}

	close(serv_sock);

	return 0;
	/* end of main */
}

void usage(char *prog) {
	INFO("usage:\n");
	INFO("\t%s [options] action\n", prog);

	INFO("options:\n");
	INFO("\t-h          --help          show this help and exit\n");
	INFO("\t-c file   --config file     read configuration from 'file'\n");

    INFO("actions:\n");
    INFO("\tstart                       start httpd\n");
    INFO("\tshutdown                    shutdown httpd\n");
    INFO("\n");
}

void exiting() {
	/* you are on the path to destruction. */
}

void die (int sig) {
	/* somebody set us up the bomb. */
	FATAL("interrupted by SIGINT, trying to quit.\n");
	++plzdie;
}
