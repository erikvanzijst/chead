#ifndef AHEAD_H
#define AHEAD_H

#include <stdlib.h>
#include <glib.h>
#include "cset.h"

typedef struct refcounter {
	cset_t *cset;
	GHashTable *nodes;
	GHashTable *based;
	int ahead;
	int behind;
} refcounter_t;

/*
 * Allocates a new refcounter_t struct, including its internal hash tables.
 * The supplied cset_t instance is deep-copied onto refcounter_t.cset.
 * The returned instance must be destroyed using `refcounter_t_destroy()`.
 */
refcounter_t * refcounter_t_new(cset_t *cset);

/*
 * Frees the memory related to the supplied refcounter_t struct.
 */
void refcounter_t_destroy(refcounter_t *refcounter);

#endif /* AHEAD_H */
