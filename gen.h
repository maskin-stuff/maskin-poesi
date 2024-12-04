#ifndef GEN_H
#define GEN_H

#include "parse.h"

int gen(FILE *f, struct ast *ast);
const char *gen_get_error_string(void);

#endif
