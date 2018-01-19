#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t write(int fd, const void *buf, size_t len)
{
	static ssize_t (*write)(int, const void *, size_t) = NULL;
	if(!write && !(write = dlsym(RTLD_NEXT, "write")))
	{
		fprintf(stderr, "dlsym: %s\n", dlerror());
		abort();
	}

	if(drand48() > 0.1)
		return write(fd, buf, len);
	errno = ENOSPC;
	return -1;
}
