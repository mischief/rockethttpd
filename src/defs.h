#ifndef DEFS_H
#define DEFS_H

#define VERSION "Rocket HTTPD 0.1.3"

#define BARK printf("[-] file %s, line %d: ", __FILE__, __LINE__)

#define BACKLOG 50 // number of connections pending in the queue

#define DEBUG // we are in fact debugging!

#endif
