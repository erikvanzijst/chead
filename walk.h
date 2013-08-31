#ifndef WALK_H
#define WALK_H

#include <stdbool.h>
#include <stdio.h>
#include "cset.h"

typedef enum { CONT, PRUNE } Continuation;

//typedef continuation (*visit_cb)(struct CSET *cset);
void walk(FILE *fp, Continuation (*)(cset_t *cset));

#endif
