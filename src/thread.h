#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "common_types.h"
#include "networking.h"
#include "http.h"
#include "defs.h"

void *dispatch_request(void *);

#endif
