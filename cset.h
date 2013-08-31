#ifndef CSET_H
#define CSET_H

typedef struct cset {
	char *sha;
	char **parents;
	int parentc;
} cset_t;

/*
 * Allocates and returns a new cset_t instance. The SHAs passed in are copied.
 */
cset_t * new_cset_t(char *sha, char **parent_shas, int parentc);

/*
 * Frees the memory related to the supplied cset_t struct.
 */
void free_cset_t(cset_t *cset);

char * format_cset_t(cset_t *cset);
#endif /* CSET_H */
