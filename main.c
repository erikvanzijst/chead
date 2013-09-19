#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "walk.h"

Continuation visitor(cset_t *cset) {
	cset_t *copy = cset_t_dup(cset);
	printf("%s\n", format_cset_t(copy));
	cset_t_destroy(copy);
	return CONT;
}

/*
 * Parses the input header. The input header are all lines before the double
 * newline.
 * This function returns the number of SHAs that were read. Consequently, this
 * number indicates the size of the pointer array that is pointed to by **shas.
 */
aheadstate_t * parse_input(FILE *f) {
	char line[1024];
	char **shas;
	GSList *slist = NULL, *el = NULL;
	int i;
	aheadstate_t *state = NULL;

	while(fgets(line, sizeof(line), f)) {
		if(strlen(g_strchomp(line)) == 0) {
			break;
		} else if(line[0] == '#') {
			continue;
		}
		slist = g_slist_append(slist, strdup(line));
	}
	shas = malloc(sizeof(char *) * g_slist_length(slist));
	el = slist;
	for (i = 0; el != NULL; el = g_slist_next(el)) {
		shas[i++] = (char *)el->data;
	}
	state = aheadstate_new(shas, i);
	g_slist_free(slist);
	free(shas);
	return state;
}

int main(int argc, char **argv) {
	GHashTable *inc = g_hash_table_new(g_str_hash, g_str_equal);
	GSList *el;
	aheadstate_t *state = parse_input(stdin);

	g_hash_table_add(inc, state->base->sha);
	for (el = state->refcounters; el != NULL; el = g_slist_next(el)) {
		g_hash_table_add(inc, ((refcounter_t *)el->data)->sha);
	}

	walk(stdin, visitor, inc);
	g_hash_table_destroy(inc);
	return 0;
}
