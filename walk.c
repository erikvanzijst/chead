#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "walk.h"

void GHCopy(gpointer key, gpointer value, GHashTable *dest) {
	g_hash_table_add(dest, g_strdup(key));
}

void walk(FILE *fp, Continuation (*visit_cb)(cset_t *cset),
		  GHashTable *includes) {

	int i;
	GHashTable *inc = g_hash_table_new_full(g_str_hash, g_str_equal,
											g_free, NULL);
	
	char *parent = NULL;
	cset_t cset;
	char *parents[40];
	cset.parents = parents;

	char line[1024];

	if(includes && g_hash_table_size(includes)) {
		g_hash_table_foreach(includes, (GHFunc)GHCopy, inc);
	}
	
	while(fgets(line, sizeof(line), fp)) {
		if(!g_hash_table_size(inc)) {
			break;
		}

		cset.sha = strtok(line, " \n");
		for(cset.parentc = 0; (parent = strtok(NULL, " \n")) != NULL;
				cset.parentc++) {
			cset.parents[cset.parentc] = parent;
		}
		
		if(g_hash_table_remove(inc, cset.sha)) {
			if(visit_cb(&cset) == CONT) {
				for(i = 0; i < cset.parentc; i++) {
					g_hash_table_add(inc, g_strdup(cset.parents[i]));
				}
			}
		}
	}
	
	g_hash_table_destroy(inc);
}
