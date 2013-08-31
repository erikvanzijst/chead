#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cset.h"

cset_t * new_cset_t(char *sha, char **parent_shas, int parentc) {
	int i = 0;
	cset_t *cset = malloc(sizeof(cset_t));

	cset->sha = strcpy(malloc(sizeof(char) * strlen(sha)), sha);
	cset->parents = malloc(sizeof(char) * parentc);
	cset->parentc = parentc;
	for(; i < parentc; i++) {
		cset->parents[i] = strcpy(malloc(sizeof(char) * strlen(parent_shas[i])),
						   		  parent_shas[i]);
	}
	return cset;
}

void free_cset_t(cset_t *cset) {
	int i = 0;
	free(cset->sha);
	for(; i < cset->parentc; i++) {
		free(cset->parents[i]);
	}
	free(cset->parents);
	free(cset);
	return;
}

char * format_cset_t(cset_t *cset) {
	char buf[1024];
	int i, pos = 0;

	pos = sprintf((char *)&buf, "cset->sha=%s (%p)", cset->sha, cset->sha);
	for(i = 0; i < cset->parentc; i++) {
		pos += sprintf(&buf[pos], " cset->parents[%d]=%s (%p)",
					  i, cset->parents[i], cset->parents[i]);
	}
	return strcpy(malloc(sizeof(char) * strlen(buf)), buf);
}
