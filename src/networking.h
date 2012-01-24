#ifndef NETWORKING_H
#define NETWORKING_H

#include "defs.h"
#include "common_types.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen);

/* unix systems programming 2nd ed, program 4.2 copyfile1.c */
int copyfile(int fromfd, int tofd);

/* thanks beej! */
int sendall(int s, char *buf, int *len);

void print_con_dat(connection_data *c);

#endif
