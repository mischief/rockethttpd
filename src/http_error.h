#ifndef HTTP_ERROR_H
#define HTTP_ERROR_H

#include "defs.h"
#include "common_types.h"
#include "http_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_code_to_data(http_data_out *);

/*	helper function to the above, it fills out the data/header
	based on the http code. */

void fill_error_template(char *buf, http_data_out *out, http_status_code code, char *code1, char *code2);

#endif
