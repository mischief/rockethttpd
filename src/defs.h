#ifndef DEFS_H
#define DEFS_H

#define PROGRAM	"Rocket HTTPD"
#define VERSION	"v0.1.3"
#define AUTHOR	"mischief"
#define EMAIL	"<this.is.srs.bsns@gmail.com>"
#define SOURCE	"http://code.google.com/p/rockethttpd/"
#define VERSTRING PROGRAM " " VERSION " by " AUTHOR " " EMAIL " " SOURCE

// we are in fact debugging!
#define DEBUG 1

/* used for debugging stuff */
#define BARK if(DEBUG) printf("[-] file \"%s\", line %d: ", __FILE__, __LINE__)

// number of connections pending in the queue
#define BACKLOG 50

#endif
