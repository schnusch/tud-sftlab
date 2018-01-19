#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static struct {
	int   (*fputs)    (const char *, FILE *);
	FILE *(*fopen)    (const char *, const char *);
	FILE *(*fdopen)   (int, const char *);
	FILE *(*freopen)  (const char *, const char *, FILE *);
	int   (*fclose)   (FILE *);
	int   (*nanosleep)(const struct timespec *, struct timespec *);
} funcs;
static sigjmp_buf jmpenv;
static FILE **open_fps = NULL;

__attribute__((constructor))
static void init(void)
{
#define X(f)  ((funcs.f = dlsym(RTLD_NEXT, #f)) == NULL \
		? (fprintf(stderr, "cannot find %s: %s\n", #f, dlerror()), abort()) : (void)0)
	X(fputs);
	X(fopen);
	X(fdopen);
	X(freopen);
	X(fclose);
	X(nanosleep);
#undef X
}

static FILE *add_fp(FILE *fp)
{
	if(!fp)
		return NULL;
	size_t len = 1;
	if(open_fps)
		while(open_fps[len++]) {}
	len++;
	void *tmp = realloc(open_fps, len * sizeof(*open_fps));
	if(!tmp)
	{
		int errnum = errno;
		fclose(fp);
		errno = errnum;
		return NULL;
	}
	open_fps = tmp;
	open_fps[len - 2] = fp;
	open_fps[len - 1] = NULL;
	return fp;
}

static void remove_fp(FILE *fp)
{
	if(!open_fps)
		return;
	size_t i = 0;
	while(open_fps[i] && open_fps[i] != fp)
		i++;
	for(; open_fps[i]; i++)
		open_fps[i] = open_fps[i + 1];
}

static int has_fp(FILE *fp)
{
	if(open_fps)
		for(size_t i = 0; open_fps[i]; i++)
			if(open_fps[i] == fp)
				return 1;
	return 0;
}

static int sigsegverr;
static void sigsegv_handler(int signum)
{
	(void)signum;
	siglongjmp(jmpenv, sigsegverr);
}

static int check_fp(FILE *fp, const char *s)
{
	if(fflush(fp) < 0)
	{
		perror("fflush");
		abort();
	}

	pid_t child = fork();
	if(child == 0)
	{
		fp = freopen("/dev/null", "wb", fp);
		if(!fp)
			_exit(1);
		funcs.fputs(s, fp);
		_exit(0);
	}
	else if(child == -1)
	{
		perror("fork");
		abort();
	}

	int status;
	do
	{
		if(waitpid(child, &status, 0) != child)
		{
			perror("waitpid");
			abort();
		}
	}
	while(WIFSTOPPED(status) || WIFCONTINUED(status));

	if(WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
	{
		fprintf(stderr, ">>> our forked friend died\n");
		return -1;
	}
	else
		return 0;
}

int fputs(const char *s, FILE *fp)
{
//	if(check_fp(fp, s) < 0)
//	{
//		errno = ENOTSUP;
//		return EOF;
//	}
//	return funcs.fputs(s, fp);

	static const struct sigaction act = {
		.sa_handler = sigsegv_handler
	};
	struct sigaction oldact;

	int ret;
	int segvjmp = sigsetjmp(jmpenv, 1);
	if(segvjmp == 0)
	{
		if(!has_fp(fp))
		{
			errno = EIO;
			return EOF;
		}

		sigsegverr = EFAULT;
		if(sigaction(SIGSEGV, &act, &oldact) < 0)
			return EOF;

		volatile char buf[sizeof(*fp) + 256];
		for(volatile char *s = (char *)fp, *d = buf; d < buf + sizeof(buf); s++, d++)
		{
			*d = *s;
			*s = *d;
		}

		sigsegverr = ELOOP;

		volatile size_t n = strlen(s);

		sigsegverr = ENOTSUP;

		ret = funcs.fputs(s, fp);
	}

	sigaction(SIGSEGV, &oldact, NULL);

	if(segvjmp != 0)
	{
		errno = segvjmp;
		ret = EOF;
	}

	return ret;
}

FILE *fopen(const char *name, const char *mode)
{
	return add_fp(funcs.fopen(name, mode));
}

FILE *fdopen(int fd, const char *mode)
{
	return add_fp(funcs.fdopen(fd, mode));
}

FILE *freopen(const char *name, const char *mode, FILE *fp)
{
	FILE *newfp = funcs.freopen(name, mode, fp);
	if(newfp == fp || !newfp)
		return newfp;
	remove_fp(fp);
	return add_fp(newfp);
}

int fclose(FILE *fp)
{
	remove_fp(fp);
	return funcs.fclose(fp);
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{
	static const long delay = 10000000;
	struct timespec req2;
	memcpy(&req2, req, sizeof(*req));
	if(req2.tv_nsec < 1000000000 - delay)
		req2.tv_nsec += delay;
	else
		req2.tv_sec++, req2.tv_nsec -= 1000000000 - delay;
	return funcs.nanosleep(&req2, rem);
}
