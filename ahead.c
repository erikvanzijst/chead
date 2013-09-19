#include "ahead.h"

refcounter_t * refcounter_t_new(char *sha) {
	refcounter_t *refcounter = malloc(sizeof(refcounter_t));
	refcounter->nodes = g_hash_table_new_full((GHashFunc)cset_t_hash,
											  (GEqualFunc)cset_t_equal,
										 	  (GDestroyNotify)cset_t_destroy,
											  NULL);
	refcounter->based = g_hash_table_new_full((GHashFunc)cset_t_hash,
											  (GEqualFunc)cset_t_equal,
											  (GDestroyNotify)cset_t_destroy,
											  NULL);
	refcounter->sha = g_strdup(sha);
	refcounter->ahead = refcounter->behind = 0;
	return refcounter;
}

void refcounter_t_destroy(refcounter_t *refcounter) {
	free(refcounter->sha);
	g_hash_table_destroy(refcounter->nodes);
	g_hash_table_destroy(refcounter->based);
	free(refcounter);
}
