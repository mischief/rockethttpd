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
#include <stdlib.h>
#include <errno.h>

/* here we read the contents of the directory
 * dir, and write a nice layout in buf. */

const int make_dir_list(connection *ret);

const char *icon_from_fname(const char *filename);

int noparent(const struct dirent *entry);

int yesparent(const struct dirent *entry);

#endif
