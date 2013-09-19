#include "ahead.h"

refcounter_t * refcounter_t_new(cset_t *cset) {
	refcounter_t *refcounter = malloc(sizeof(refcounter_t));
	refcounter->nodes = g_hash_table_new_full((GHashFunc)cset_t_hash,
											  (GEqualFunc)cset_t_equal,
										 	  (GDestroyNotify)cset_t_destroy,
											  NULL);
	g_hash_table_add(refcounter->nodes, cset_t_dup(cset));
	refcounter->based = g_hash_table_new_full((GHashFunc)cset_t_hash,
											  (GEqualFunc)cset_t_equal,
											  (GDestroyNotify)cset_t_destroy,
											  NULL);
	refcounter->cset = cset_t_dup(cset);
	refcounter->ahead = refcounter->behind = 0;
	return refcounter;
}

void refcounter_t_destroy(refcounter_t *refcounter) {
	cset_t_destroy(refcounter->cset);
	g_hash_table_destroy(refcounter->nodes);
	g_hash_table_destroy(refcounter->based);
	free(refcounter);
}
