#ifndef UTIL_H
#define UTIL_H

#define LEN(X) (sizeof(X) / sizeof((X)[0]))

void *ecalloc(size_t nmemb, size_t size);
void *emalloc(size_t size);
void *erealloc(void *p, size_t size);
char *estrdup(char *s);
int estrcmp(const char *s1, const char *s2);

char **str_split(const char *s, const char a_delim);
int count_chars(char *s, char ch);
int file_exists(const char *fname);

void die(int eval, const char *fmt, ...);

#endif
