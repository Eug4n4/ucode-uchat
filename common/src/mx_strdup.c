#include "common.h"

char *mx_strnew(const int size) {
	if (size >= 0) {
		char *str = malloc(size + 1);
		if (str) {
			for (int i = 0; i <= size; i++) {
				str[i] = '\0';
			}
			return str;
		}
	}
	return NULL;
	
}

char *mx_strndup(const char *s1, size_t n) {
	size_t len = strlen(s1);

	if (len > n) {
		char *str = mx_strnew(n);

		if (str) {
			return strncpy(str, s1, n);
		}
	} else {
		char *str = mx_strnew(len);

		if (str) {
			return strncpy(str, s1, len);
		}
	}
	return NULL;
}


char *mx_strdup(const char *s)
{
	return mx_strndup(s, strlen(s));
}



