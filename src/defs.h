#ifndef DEFS_H
#define DEFS_H

#define PROGRAM	"Rocket HTTPD"
#define VERSION	"v0.2.0"
#define AUTHOR	"mischief"
#define EMAIL	"<mischief@offblast.org>"
#define SOURCE	"https://github.com/mischief/rockethttpd/"
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
