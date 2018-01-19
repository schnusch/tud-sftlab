#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t read(int fd, void *buf, size_t len)
{
	static ssize_t (*read)(int, void *, size_t) = NULL;
	if(!read && !(read = dlsym(RTLD_NEXT, "read")))
	{
		fprintf(stderr, "dlsym: %s\n", dlerror());
		abort();
	}

	if(drand48() > 0.1)
		return read(fd, buf, len);
	errno = EINTR;
	return -1;
}
