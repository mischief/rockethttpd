#ifndef DEFS_H
#define DEFS_H

#define PROGRAM	"Rocket HTTPD"
#define VERSION	"v0.1.7"
#define AUTHOR	"mischief"
#define EMAIL	"<this.is.srs.bsns@gmail.com>"
#define SOURCE	"http://code.google.com/p/rockethttpd/"
#define VERSTRING PROGRAM " " VERSION " by " AUTHOR " " EMAIL " " SOURCE

// set DEBUG in Makefile
// #define DEBUG 0

#define BARK(fmt, ...) if(DEBUG) printf("[-] file \"%s\", line %d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__); fflush(NULL)

// listen() queue
#define BACKLOG 50

#endif
