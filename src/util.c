#include <assert.h>
#include <string.h>
#include <stdlib.h>

char *
strdup(const char *s) {
	char *d = malloc(strlen(s) + 1);
	if (d == NULL) return NULL;
	strcpy(d, s);
	return d;
}

char **
str_split(char* a_str, const char a_delim) {
	char** result    = 0;
	size_t count     = 0;
	char* last_delim = 0;
	char delim[2] = { a_delim, 0 }; /* converet a_delim into string for strtok */

	/* count number of elements that will be extracted. */
	for (char *tmp = a_str; *tmp; tmp++)
		if (a_delim == *tmp) {
			count++;
			last_delim = tmp;
		}

	/* add space for trailing token. */
	count += last_delim < (a_str + strlen(a_str) - 1);

	/* add space for terminating null string so caller
	 * knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char*) * count);

	if (result) {
		size_t idx  = 0;
		char* token = strtok(a_str, delim);

		while (token) {
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
}
