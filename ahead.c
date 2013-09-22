#include "ahead.h"

/*
 * Used internally by `aheadandbehind` and implements the algorithm.
 */
bool visit(cset_t *cset, aheadstate_t *state);

refcounter_t * refcounter_t_new(char *sha) {
	refcounter_t *refcounter = malloc(sizeof(refcounter_t));
	refcounter->nodes = g_hash_table_new_full((guint (*)(gconstpointer v))sha_hash,
											  g_str_equal,
										 	  free,
											  NULL);
	g_hash_table_add(refcounter->nodes, g_strdup(sha));
	refcounter->based = g_hash_table_new_full((guint (*)(gconstpointer v))sha_hash,
											  g_str_equal,
											  free,
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
    char **shas;
    int shaslen;
    int i = 0;
	GSList *el;
    cset_t *cset;
    walker_t *walker;
	aheadstate_t internalstate;
    
	internalstate.base = state->base;
	internalstate.refcounters = g_slist_copy(state->refcounters);
    shaslen = 1 + g_slist_length(state->refcounters);
    shas = malloc(sizeof(char *) * shaslen);

    shas[i++] = state->base->sha;
    for (el = state->refcounters; el != NULL; el = g_slist_next(el)) {
        shas[i++] = ((refcounter_t *)el->data)->sha;
    }
    walker = walker_new(stdin, shas, shaslen);
    while((cset = walker_next(walker))) {
        // printf("%s\n", format_cset_t(cset));
        if (!visit(cset, &internalstate)) {
            break;
        }
    }

    walker_destroy(walker);
	g_slist_free(internalstate.refcounters);
	return state;
}

void addparents(GHashTable *ht, cset_t *cset) {
    int i;
    
    for (i = 0; i < cset->parentc; i++) {
        g_hash_table_add(ht, g_strdup(cset->parents[i]));
    }
}

gboolean find(char *sha, char *shavalue, GHashTable *ht) {
    return !g_hash_table_contains(ht, sha);
}

bool difference(GHashTable *ht1, GHashTable *ht2) {
    return g_hash_table_find(ht1, (gboolean (*)(gpointer key, gpointer value, gpointer user_data))find, ht2) != NULL;
}

char * format_ht(GHashTable *ht) {
    char *shas[1 + g_hash_table_size(ht)];
    int i = 0;
    GList *el;
    GList *keys = g_hash_table_get_keys(ht);
    
    for (el = keys; el != NULL; el = g_list_next(el)) {
        shas[i++] = (char *)el->data;
    }
    shas[i] = NULL;
    g_list_free(keys);
    return g_strjoinv(", ", shas);
}

bool visit(cset_t *cset, aheadstate_t *state) {
	gboolean onmain;
    GSList *next, *el;
    refcounter_t *ref;

	if (state->refcounters == NULL) {
		return FALSE;
	}

	onmain = g_hash_table_remove(state->base->nodes, cset->sha);
    if (onmain) {
        addparents(state->base->nodes, cset);
    }
    // printf("onmain: %d, sha: %s, base->nodes: [%s]\n", onmain, cset->sha, format_ht(state->base->nodes));

    for (el = state->refcounters; el != NULL; ) {
        ref = (refcounter_t *)el->data;
        if (g_hash_table_remove(ref->nodes, cset->sha)) {
            if (onmain) {
                addparents(ref->based, cset);
            } else {
                ref->ahead++;
                addparents(ref->nodes, cset);
            }
            if (g_hash_table_size(ref->nodes) == 0 &&
                !difference(state->base->nodes, ref->based)) {
                // printf("difference (%d / %d): %d\n", g_hash_table_size(state->base->nodes), g_hash_table_size(ref->based), difference(state->base->nodes, ref->based));
                // printf("base: [%s], ref: [%s]\n", format_ht(state->base->nodes), format_ht(ref->based));
                next = el->next;
                state->refcounters = g_slist_delete_link(state->refcounters, el);
                el = next;
                continue;
            }
        } else if (g_hash_table_remove(ref->based, cset->sha)) {
            addparents(ref->based, cset);
            if (!difference(state->base->nodes, ref->based)) {
                next = el->next;
                state->refcounters = g_slist_delete_link(state->refcounters, el);
                el = next;
                continue;
            }
        } else if (onmain) {
            ref->behind++;
        }
        el = el->next;
    }
	return TRUE;
}

void print_ahead_state(aheadstate_t *state) {
    GSList *el;
    refcounter_t *ref;
    
    for (el = state->refcounters; el != NULL; el = g_slist_next(el)) {
        ref = (refcounter_t *)el->data;
        printf("%s: %d ahead / %d behind\n", ref->sha, ref->ahead, ref->behind);
    }
}
