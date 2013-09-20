#ifndef WALK_H
#define WALK_H

#include <stdbool.h>
#include <stdio.h>
#include <glib.h>
#include "cset.h"

typedef enum { CONT, PRUNE } Continuation;

/*
 * Performs a walk over the rev-list provided by *fp.
 * `ctx` provides for the caller to pass context data to the visitor at
 * runtime.
 */
void walk(FILE *fp, Continuation (*)(cset_t *cset, void *ctx),
          GHashTable *includes, void *ctx);

#endif
