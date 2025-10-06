#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

char *strndup(const char *str, size_t n)
{
	char *buf = malloc(n+1);
	memcpy(buf, str, n);
	buf[n] = 0;
	return buf;
}

#endif
