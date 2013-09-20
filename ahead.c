#include "ahead.h"

refcounter_t * refcounter_t_new(char *sha) {
	refcounter_t *refcounter = malloc(sizeof(refcounter_t));
	refcounter->nodes = g_hash_table_new_full((GHashFunc)g_str_hash,
											  (GEqualFunc)g_str_equal,
										 	  (GDestroyNotify)free,
											  NULL);
	g_hash_table_add(refcounter->nodes, g_strdup(sha));
	refcounter->based = g_hash_table_new_full(g_str_hash,
											  g_str_equal,
											  (GDestroyNotify)free,
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

aheadstate_t * aheadstate_new(char **shas, int num_shas) {
	int i;

	if (num_shas < 1) {
		return NULL;
	}
	aheadstate_t *state = malloc(sizeof(aheadstate_t));
	state->base = refcounter_t_new(shas[0]);
	state->refcounters = NULL;
	for (i = 1; i < num_shas; i++) {
		state->refcounters = g_slist_append(state->refcounters,
		                                    refcounter_t_new(shas[i]));
	}
	return state;
}

void aheadstate_destroy(aheadstate_t *state) {
	refcounter_t_destroy(state->base);
	g_slist_free_full(state->refcounters, (GDestroyNotify)refcounter_t_destroy);
}

aheadstate_t * aheadandbehind(FILE *fp, aheadstate_t *state) {
	GHashTable *inc = g_hash_table_new(g_str_hash, g_str_equal);
	GSList *el;
	aheadstate_t internalstate;
	internalstate.base = state->base;
	internalstate.refcounters = g_slist_copy(state->refcounters);


	g_hash_table_add(inc, state->base->sha);
	for (el = state->refcounters; el != NULL; el = g_slist_next(el)) {
		g_hash_table_add(inc, ((refcounter_t *)el->data)->sha);
	}

	walk(fp, (Continuation (*)(cset_t *cset, void *ctx))aheadvisitor, inc,
		 &internalstate);

	g_slist_free(internalstate.refcounters);
	g_hash_table_destroy(inc);
	return state;
}

Continuation aheadvisitor(cset_t *cset, aheadstate_t *state) {
	printf("%s\n", format_cset_t(cset));
	return CONT;
}
