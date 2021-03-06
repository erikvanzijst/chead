#include "cset.h"

cset_t * cset_t_new(char *sha, char **parent_shas, int parentc) {
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

cset_t * cset_t_dup(cset_t *cset) {
	return cset_t_new(cset->sha, cset->parents, cset->parentc);
}

void cset_t_destroy(cset_t *cset) {
	int i = 0;
	free(cset->sha);
	for(; i < cset->parentc; i++) {
		free(cset->parents[i]);
	}
	free(cset->parents);
	free(cset);
	return;
}

guint cset_t_hash(cset_t *cset) {
	return sha_hash(cset->sha);
}

gboolean cset_t_equal(cset_t *cset1, cset_t *cset2) {
	return g_str_equal(cset1->sha, cset2->sha);
}

int to_int(char c) {
    int i = (int)c;
    if (i >= 48 && i <= 57)
        return i - 48;
    if (i >= 65 && i <= 70)
        return 10 + (i - 65);
    if (i >= 97 && i <= 102)
        return 10 + (i - 97);
    return 0;
}

guint sha_hash(gconstpointer v) {
    const signed char *sha;
    guint32 hash = 0;
    sha = v;
    hash |= ((to_int(sha[0]) << 4) | to_int(sha[1])) << 24;
    hash |= ((to_int(sha[2]) << 4) | to_int(sha[3])) << 16;
    hash |= ((to_int(sha[4]) << 4) | to_int(sha[5])) << 8;
    hash |= ((to_int(sha[6]) << 4) | to_int(sha[7]));
    return hash;
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
