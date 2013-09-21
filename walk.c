#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "walk.h"

walker_t * walker_new(FILE *fp, char **shas, int shaslen) {
    int i;
    walker_t *state = malloc(sizeof(walker_t));
    
    state->cset = malloc(sizeof(cset_t));
    state->cset->parents = malloc(sizeof(char *) * 40);
    state->cset->parentc = 0;
    state->fp = fp;
    state->todo = g_hash_table_new_full(g_str_hash, g_str_equal,
										g_free, NULL);

    for (i = 0; i < shaslen; i++) {
        g_hash_table_add(state->todo, g_strdup(shas[i]));
    }
    return state;
}

void walker_destroy(walker_t *state) {
    g_hash_table_destroy(state->todo);
    free(state->cset->parents);
    free(state->cset);
    free(state);
}

cset_t * walker_next(walker_t *walker) {
    cset_t *cset = walker->cset;
	char *parent = NULL;
    int i;

    while (g_hash_table_size(walker->todo) &&
            fgets(walker->line, sizeof(walker->line), walker->fp)) {
		cset->sha = strtok(walker->line, " \n");
		for (cset->parentc = 0; (parent = strtok(NULL, " \n")) != NULL;
				cset->parentc++) {
			cset->parents[cset->parentc] = parent;
		}
		if (g_hash_table_remove(walker->todo, cset->sha)) {
			for (i = 0; i < cset->parentc; i++) {
				g_hash_table_add(walker->todo, g_strdup(cset->parents[i]));
			}
            return cset;
		}
    }
    return NULL;
}
