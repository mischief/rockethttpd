#ifndef DIR_H
#define DIR_H

#include <dirent.h>
#include <string.h>
#include <stdio.h>

const char dir_page[] = 
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
"<head>\n"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"
"<title>Index of %s</title>\n" /* directory we are indexing */
"</head>\n"
"<body>\n"
"\%s" /* directory listing goes here */
"</body></html>\n";

const char dir_entry[] = ""

#endif
