#ifndef VERBOSE_H_INCLUDED
#define VERBOSE_H_INCLUDED

#include <stdio.h>

static FILE *fopen2(const char *name, const char *mode)
{
	FILE *fp = fopen(name, mode);
	int errnum = errno;
	fprintf(stderr, "fopen(%s, %s) = %p\n", name, mode, (void *)fp);
	errno = errnum;
	return fp;
}
#define fopen(name, mode)  fopen2(name, mode)

static int fclose2(FILE *fp)
{
	int e = fclose(fp);
	int errnum = errno;
	fprintf(stderr, "fclose(%p) = %d\n", (void *)fp, e);
	errno = errnum;
	return e;
}
#define fclose(fp)  fclose2(fp)

static int fputs2(const char *s, FILE *fp)
{
	fprintf(stderr, "fputs(%p, %p)\n", s, (void *)fp);
	int e = fputs(s, fp);
	int errnum = errno;
	fprintf(stderr, " = %d\n", e);
	errno = errnum;
	return e;
}
#define fputs(s, fp)  fputs2(s, fp)

#ifdef FFLUSH
static int fflush2(FILE *fp)
{
	fprintf(stderr, "fflush(%p)\n", (void *)fp);
	int e = fflush(fp);
	int errnum = errno;
	fprintf(stderr, " = %d\n", e);
	errno = errnum;
	return e;
}
#define fflush(fp)  fflush2(fp)
#endif

#endif
