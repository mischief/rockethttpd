#ifndef DIR_H
#define DIR_H

#include "defs.h"
#include "common_types.h"
#include "http_helper.h"
#include "http_error.h"

#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include <dirent.h>

/* here we read the contents of the directory
 * dir, and write a nice layout in buf. */

int make_dir_list(connection *ret);

const char *icon_from_fname(const char *filename);

const char *get_suffx_from_size(unsigned long bytes, float *floaty);

int exist_index(connection *ret);

int noparent(struct dirent *entry);

int yesparent(struct dirent *entry);

#endif
