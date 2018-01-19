#ifdef FAKE_TAR_USER
 #define _GNU_SOURCE
 #include <dlfcn.h>
 #include <grp.h>
 #include <pwd.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/stat.h>
 #include <sys/stat.h>

static struct {
	int (*__fxstatat64)(int, int, const char *, struct stat64 *, int);
	int (*__fxstat64  )(int, int, struct stat64 *);
	struct passwd *(*getpwuid)(uid_t);
	struct group  *(*getgrgid)(gid_t);
} funcs;

__attribute__((constructor))
static void init(void)
{
 #define X(f)  ((funcs.f = dlsym(RTLD_NEXT, #f)) == NULL \
		? (fprintf(stderr, "cannot find %s: %s\n", #f, dlerror()), abort()) : (void)0)
	X(__fxstat64);
	X(__fxstatat64);
	X(getpwuid);
	X(getgrgid);
 #undef X
}

int __fxstat64(int ver, int fd, struct stat64 *st)
{
	int ret = funcs.__fxstat64(ver, fd, st);
	if(ret == 0)
	{
		st->st_uid = 2000;
		st->st_gid = 2000;
	}
	return ret;
}

int __fxstatat64(int ver, int fd, const char *name, struct stat64 *st, int flags)
{
	int ret = funcs.__fxstatat64(ver, fd, name, st, flags);
	if(ret == 0)
	{
		st->st_uid = 2000;
		st->st_gid = 2000;
	}
	return ret;
}

struct passwd *getpwuid(uid_t uid)
{
	static char name[] = "martin";
	static struct passwd martin = {
		.pw_name = name
	};
	if(uid == 2000)
		return &martin;
	return funcs.getpwuid(uid);
}

struct group *getgrgid(gid_t gid)
{
	static char name[] = "martin";
	static struct group martin = {
		.gr_name = name
	};
	if(gid == 2000)
		return &martin;
	return funcs.getgrgid(gid);
}
#endif
