#include <stdio.h>
#include <string.h>
#include "walk.h"

Continuation visitor(cset_t *cset) {
	printf("%s\n", format_cset_t(cset));
	return CONT;
}

int main(int argc, char **argv) {
	GHashTable *inc = g_hash_table_new(g_str_hash, g_str_equal);
	walk(stdin, visitor, inc);
	g_hash_table_destroy(inc);
	return 0;
}
