#define _XOPEN_SOURCE
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>

static struct {
	ssize_t (*read  )(int, void *, size_t);
	ssize_t (*write )(int, const void *, size_t);
	void   *(*malloc)(size_t);
	int     (*select)(int, fd_set *, fd_set *, fd_set *, struct timeval *);
#ifdef FAKE_TAR_USER
	int (*__fxstatat)(int, int, const char *, struct stat *, int);
	int (*__fxstat  )(int, int, struct stat *);
	struct passwd *(*getpwuid  )(uid_t);
	struct group  *(*getgrgid  )(gid_t);
#endif
} funcs;
