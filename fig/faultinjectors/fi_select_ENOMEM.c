#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	static int (*select)(int, fd_set *, fd_set *, fd_set *, struct timeval *) = NULL;
	if(!select && !(select = dlsym(RTLD_NEXT, "select")))
	{
		fprintf(stderr, "dlsym: %s\n", dlerror());
		abort();
	}

	if(drand48() > 0.1)
		return select(nfds, readfds, writefds, exceptfds, timeout);
	errno = ENOMEM;
	return -1;
}
