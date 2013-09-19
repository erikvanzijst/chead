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

void parse_input(FILE *f, char **base, GHashTable *includes) {
	char line[1024];

	*base = NULL;
	while(fgets(line, sizeof(line), f)) {
		if(strlen(g_strchomp(line)) == 0) {
			break;
		} else if(line[0] == '#') {
			continue;
		}
		if(*base == NULL) {
			// first line is the base ref
			*base = strdup(line);
		}
		g_hash_table_add(includes, strdup(line));
	}
}

int main(int argc, char **argv) {
	GHashTable *inc = g_hash_table_new(g_str_hash, g_str_equal);
	char *base;

	parse_input(stdin, &base, inc);

	walk(stdin, visitor, inc);
	g_hash_table_destroy(inc);
	return 0;
}
