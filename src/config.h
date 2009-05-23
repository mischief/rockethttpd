#ifndef CONFIG_H
#define CONFIG_H

#include "defs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/* null terminate an array of these */
typedef struct {
	char *name;
	void *data;
	size_t size;
	char type;
} config_item;

int parse_config_file(const char *path, config_item *conf);

#endif
