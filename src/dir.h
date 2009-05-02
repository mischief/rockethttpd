#ifndef DIR_H
#define DIR_H

#include <dirent.h>
#include <string.h>
#include <stdio.h>

/* here we read the contents of the directory
 * dir, and write a nice layout in buf. */

const char *make_dir_list(char *buf, const char *dir);

#endif
