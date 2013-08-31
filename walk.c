#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "walk.h"

void walk(FILE *fp, Continuation (*visit_cb)(cset_t *cset)) {
	char line[1024];
	char *sha = NULL;
	cset_t cset;
	char *parents[40];

	cset.parents = parents;
	while(fgets(line, sizeof(line), fp)) {

		cset.sha = strtok(line, " \n");
		for(cset.parentc = 0; (sha = strtok(NULL, " \n")) != NULL; cset.parentc++) {
			cset.parents[cset.parentc] = sha;
		}
		if(visit_cb(&cset) == PRUNE) {
			break;
		}
	}
}
