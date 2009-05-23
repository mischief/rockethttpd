#ifndef DEFS_H
#define DEFS_H

#define PROGRAM	"Rocket HTTPD"
#define VERSION	"v0.1.8"
#define AUTHOR	"mischief"
#define EMAIL	"<this.is.srs.bsns@gmail.com>"
#define SOURCE	"http://code.google.com/p/rockethttpd/"
#define VERSTRING PROGRAM " " VERSION " by " AUTHOR " " EMAIL " " SOURCE

#define UNIX_NAME "rockethttpd"

#include <assert.h>

#if DEBUG
#define ERROR(fmt, ...) fprintf(stderr, "[e] '%s' %d: error: " fmt, __FILE__, __LINE__, ##__VA_ARGS__); fflush(NULL)
#else
#define ERROR(...)
#endif

#define FATAL(fmt, ...) fprintf(stderr, "[f] '%s' %d: fatal: " fmt, __FILE__, __LINE__, ##__VA_ARGS__); fflush(NULL)

#define INFO(fmt, ...) fprintf(stdout, "[i] " fmt, ##__VA_ARGS__); fflush(NULL)

/* listen() queue */
#define BACKLOG 50

#endif
