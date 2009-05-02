#include "networking.h"

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen) {
	switch(sa->sa_family) {
		case AF_INET:
			inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
			s, maxlen);
			break;

		case AF_INET6:
			inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
			s, maxlen);
			break;

		default:
		strncpy(s, "Unknown AF", maxlen);
    }

    return s;
}

int sendall(int s, char *buf, int *len) {
	int total = 0;        // how many bytes we've sent
	int bytesleft = *len; // how many we have left to send
	int n = 0;

	while(total < *len) {
		n = send(s, buf+total, bytesleft, MSG_NOSIGNAL);
		if (n <= 0) {
			// well, either the connection died or we have an error.
			break;
		}
		total += n;
		bytesleft -= n;
	}

	*len = total; // return number actually sent here

	return n==-1?-1:0; // return -1 on failure, 0 on success
}

void print_con_dat(connection_data *c) {
	printf("[+] %s:%s <-> %s:%s > ", c->serverip, c->port, c->clientip, c->clientport);
	return;
}
