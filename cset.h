#ifndef CSET_H
#define CSET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

typedef struct cset {
	char *sha;
	char **parents;
	int parentc;
} cset_t;

/*
 * Allocates and returns a new cset_t instance. The SHAs passed in are copied.
 */
cset_t * cset_t_new(char *sha, char **parent_shas, int parentc);

/*
 * Deep-copies the supplied cset_t instance.
 * The returned pointer must be freed using cset_t_free().
 */
cset_t * cset_t_dup(cset_t *cset);

/*
 * Frees the memory related to the supplied cset_t struct.
 */
void cset_t_destroy(cset_t *cset);

/*
 * Computes the hash value for a cset_t. For use in g_hash_table.
 */
guint cset_t_hash(cset_t *cset);

/*
 * Compares two instances of cset_t. For use in g_hash_table.
 */
gboolean cset_t_equal(cset_t *cset1, cset_t *cset2);

char * format_cset_t(cset_t *cset);
#endif /* CSET_H */
