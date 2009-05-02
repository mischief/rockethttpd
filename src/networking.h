#ifndef NETWORKING_H
#define NETWORKING_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "common_types.h"

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen);

/* thanks beej! */

int sendall(int s, char *buf, int *len);

void print_con_dat(connection_data *c);

#endif
