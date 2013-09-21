#ifndef AHEAD_H
#define AHEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "cset.h"
#include "walk.h"

typedef struct refcounter {
	char *sha;
	GHashTable *nodes;
	GHashTable *based;
	int ahead;
	int behind;
} refcounter_t;

typedef struct aheadstate {
	refcounter_t *base;
	GSList *refcounters;
} aheadstate_t;

char * format_ht(GHashTable *ht);

/*
 * Computes the ahead and behind numbers for the refs contained in `state`
 * based on the rev-list provided by `fp`.
 */
aheadstate_t * aheadandbehind(FILE *fp, aheadstate_t *state);

void print_ahead_state(aheadstate_t *state);

/*
 * Allocates a new refcounter_t struct, including its internal hash tables.
 * The supplied sha string is deep-copied onto refcounter_t.sha.
 * The returned instance must be destroyed using `refcounter_t_destroy()`.
 */
refcounter_t * refcounter_t_new(char *sha);

/*
 * Frees the memory related to the supplied refcounter_t struct.
 */
void refcounter_t_destroy(refcounter_t *refcounter);

/*
 * Allocates a new aheadstate_t instance, initialized with ref counters for
 * the specified shas.
 * Note that the first sha in the list is the base ref against which the ahead
 * and behind count of the others are calculated.
 *
 * The returned instance must be freed using aheadstate_destroy().
 */
aheadstate_t * aheadstate_new(char **shas, int num_shas);

/*
 * Frees the memory related to the specified aheadstate_t instance.
 */
void aheadstate_destroy(aheadstate_t *state);

#endif /* AHEAD_H */
