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
int parse_input(FILE *f, char ***shas) {
	char line[1024];
	GSList *slist = NULL, *el = NULL;
	int i;

	while(fgets(line, sizeof(line), f)) {
		if(strlen(g_strchomp(line)) == 0) {
			break;
		} else if(line[0] == '#') {
			continue;
		}
		slist = g_slist_append(slist, strdup(line));
	}
	*shas = malloc(sizeof(char *) * g_slist_length(slist));
	el = slist;
	for (i = 0; el != NULL; el = g_slist_next(el)) {
		(*shas)[i++] = (char *)el->data;
	}
	g_slist_free(slist);
	return i;
}

int main(int argc, char **argv) {
	GHashTable *inc = g_hash_table_new(g_str_hash, g_str_equal);
	char **shas;
	int i = 4;
	int cnt;

	cnt = parse_input(stdin, &shas);
	for (i = 0; i < cnt; i++) {
		printf("%s\n", shas[i]);
	}

	// walk(stdin, visitor, inc);
	g_hash_table_destroy(inc);
	return 0;
}
