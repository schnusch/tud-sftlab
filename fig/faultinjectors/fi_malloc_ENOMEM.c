#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void *malloc(size_t size)
{
	static void *(*malloc)(size_t) = NULL;
	if(!malloc && !(malloc = dlsym(RTLD_NEXT, "malloc")))
	{
		fprintf(stderr, "dlsym: %s\n", dlerror());
		abort();
	}

	if(drand48() > 0.1)
		return malloc(size);
	errno = ENOMEM;
	return NULL;
}
