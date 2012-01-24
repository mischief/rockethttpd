#ifndef THREAD_H
#define THREAD_H

#include "defs.h"
#include "common_types.h"
#include "networking.h"
#include "http.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

//#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/socket.h>

void *dispatch_request(void *);

#endif
