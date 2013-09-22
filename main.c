#include "main.h"

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
    struct timeval ts;
    double start;
    
    aheadstate_t *state = parse_input(stdin);
    gettimeofday(&ts, NULL);
    start = (double)ts.tv_sec + (double)ts.tv_usec / 1000000.0;
    state = aheadandbehind(stdin, state);
    gettimeofday(&ts, NULL);

    print_ahead_state(state);
    printf("runtime: %.3f seconds\n",
           ((double)ts.tv_sec + (double)ts.tv_usec / 1000000.0) - start);
    aheadstate_destroy(state);
	return 0;
}
