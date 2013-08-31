#include <stdio.h>
#include <string.h>
#include "walk.h"

Continuation visitor(cset_t *cset) {
	printf("%s\n", format_cset_t(cset));
	return CONT;
}

int main(int argc, char **argv) {
	walk(stdin, visitor);
	return 0;
}
