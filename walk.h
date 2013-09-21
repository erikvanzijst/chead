#ifndef WALK_H
#define WALK_H

#include <stdbool.h>
#include <stdio.h>
#include <glib.h>
#include "cset.h"

typedef enum { CONT, PRUNE } Continuation;

typedef struct walker {
    FILE *fp;
    GHashTable *todo;
    cset_t *cset;
    char line[1024];
} walker_t;

walker_t * walker_new(FILE *fp, char **shas, int shaslen);

void walker_destroy(walker_t *state);

cset_t * walker_next(walker_t *walker);

#endif
