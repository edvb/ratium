#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int
estrcmp(const char *s1, const char *s2) {
	if (s1 == NULL || s2 == NULL)
		return 1;
	for (; *s1 == *s2; s1++, s2++)
		if (*s1 == '\0')
			return 0;
	return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}

char *
strdup(const char *s) {
	char *d = malloc(strlen(s) + 1);
	if (d == NULL) return NULL;
	strcpy(d, s);
	return d;
}

char **
str_split(const char *s, const char a_delim) {
	char** result    = 0;
	size_t count     = 0;
	char* last_delim = 0;
	char delim[2] = { a_delim, 0 }; /* converet a_delim into string for strtok */
	char *a_str = malloc((strlen(s)+1) * sizeof(char));
	strcpy(a_str, s);

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

	result = malloc(count * sizeof(char*));

	if (result) {
		size_t idx  = 0;
		char *token = strtok(a_str, delim);

		while (token) {
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}

	free(a_str);
	return result;
}

int
count_chars(char *s, char ch) {
	int count = 0;
	int len = strlen(s);
	for (int i = 0; i < len; i++)
		if (s[i] == ch)
			count++;
	return count;
}

/* checks if file fname can be read */
int
file_exists(const char *fname) {
	FILE *file;
	if ((file = fopen(fname, "r")) != NULL) {
		fclose(file);
		return 1;
	}
	return 0;
}

void
die(int eval, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	if (eval > -1)
		exit(eval);
}
