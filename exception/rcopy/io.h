#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#ifdef __cplusplus
	#include <cstdio>
#else
	#include <stdio.h>
#endif
#include <sys/types.h>

static inline ssize_t fread2(FILE *fp, char *buf, size_t n)
{
	size_t m = fread(buf, 1, n, fp);
	if(m == 0)
	{
		if(!ferror(fp))
			return 0;
		clearerr(fp);
		return -1;
	}
	return m;
}

static inline ssize_t fwrite2(FILE *fp, const char *buf, size_t n)
{
	size_t m = fwrite(buf, 1, n, fp);
	if(m == 0 && n > 0)
	{
		(void)ferror(fp);
		clearerr(fp);
		return -1;
	}
	return m;
}

static inline int freadall(FILE *fp, char *buf, size_t *pn)
{
	char   *p = buf;
	size_t  n = *pn;
	ssize_t m = 0;
	while(n > 0)
	{
		m = fread2(fp, p, n);
		if(m <= 0)
			break;
		p += m;
		n -= m;
	}
	*pn = p - buf;
	return m < 0 ? -1 : 0;
}

static inline int fwriteall(FILE *fp, const char *buf, size_t *pn)
{
	const char *p = buf;
	size_t      n = *pn;
	ssize_t     m = 0;
	while(n > 0)
	{
		m = fwrite2(fp, p, n);
		if(m <= 0)
			break;
		p += m;
		n -= m;
	}
	*pn = p - buf;
	return m < 0 ? -1 : 0;
}

#endif
