#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		die(1, "calloc:");

	return p;
}

void *
emalloc(size_t size)
{
	void *p;

	if (!(p = malloc(size)))
		die(1, "malloc:");

	return p;
}

void *
erealloc(void *p, size_t size)
{
	if (!(p = realloc(p, size)))
		die(1, "realloc:");

	return p;
}

char *
estrdup(char *s)
{
	if (!(s = strdup(s)))
		die(1, "strdup:");

	return s;
}

void
efree(void *p)
{
	if (p)
		free(p);
}

int
estrcmp(const char *s1, const char *s2) {
	if (s1 == NULL || s2 == NULL)
		return 1;
	for (; *s1 == *s2; s1++, s2++)
		if (*s1 == '\0')
			return 0;
	return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}

/* split string s into muiltple strings by a_delim */
char **
str_split(const char *s, const char a_delim)
{
	if (!s) die(1, "strsplit: given null pointer");

	char **ret = 0;
	size_t count = 0;
	char *last_delim = 0;
	char delim[2] = { a_delim, 0 }; /* converet a_delim into string for strtok */
	char *a_str = ecalloc(strlen(s)+1, sizeof(char));
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

	ret = ecalloc(count, sizeof(char*));

	if (ret) {
		size_t idx  = 0;
		char *token = strtok(a_str, delim);

		while (token) {
			assert(idx < count);
			*(ret + idx++) = estrdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(ret + idx) = 0;
	}

	free(a_str);
	return ret;
}

/* counts the number of times ch appears in s */
int
count_chars(char *s, char ch) {
	if (s == NULL) return 0;
	int count = 0;
	for (int i = 0; i < strlen(s); i++)
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
