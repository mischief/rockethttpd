#include "config.h"

int parse_config_file(const char *path, config_item *conf) {
	FILE *f;
	char buf[256];
	char id[32];
	char data[100];
	int line = 0;
	config_item *item;
	int i = 0;

	if( (f = fopen(path, "r")) == NULL) {
		ERROR("fopen('%s'): %s\n", path, strerror(errno));
		return -1;
	}

	/* du. du hast. du hast mich. */
	do {

		/* read a line from the file.
		 * keep going until we can't read any more */
		if( !fgets(buf, sizeof(buf), f) )
			break;

		++line;

		/* if the first char is a #, it's a comment, don't waste time scanning it */
		if(buf[0] == '#') continue;

		/* if there is a # in the string it is a comment, turn # into NULL */
		for( i=0; buf[i] && buf[i]!='#'; i++ );
		buf[i] = 0;

		/* scan the buffer, if there is no match,
		 * get the next line. */
		if(sscanf(buf,"%32s = %s", id, data) == EOF) continue;

		/* sanity checks */
		if( (id[0] && !data[0]) || (!id[0] && data[0]) ) {
			ERROR("bad config data on line %d\n", line);
	    	return -1;
		}

		if( !id[0] && !data[0] ) {
			ERROR("configuration option could not be read on line %d\n", line);
	    	continue;
		}

		item = conf;

		while(1) {
			/* if we reach the sentinel, then it wasn't
			 * found in the options list. inform ze user. */
			if(item->name == NULL) {
				ERROR("bad configuration item, line %d: '%s'\n", line, id);
				return -1;
				}
			/* check if current item id is the same as what
			 * we just read.  if it is, read to the
			 * pointer and break. */
			if( !strcasecmp(id, item->name) ) {
				switch(item->type) {
					/* int type */
					case 'i':
						*(int*)(item->data) = atoi(data);
						break;
					/* c string */
					case 's':
						strncpy((char*)(item->data), data, item->size);
						break;
					/* size_t */
					case 'z':
						sscanf(data, "%zu", (size_t*)(item->data) );
					default:
						break;
					}
				break;
				}
			/* next in line please. */
			++item;
		}

	} while (1);

	fclose(f);

	return 0;
}
