#ifdef FILETOOSMALL
 #define NOTOOLS
#endif
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#ifdef VALGRIND
 #include <limits.h>
#endif
#include <stdint.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "table.h"
#include "tests.h"
#ifndef NOTOOLS
 #include "tools.h"
#endif
#include "stats.h"
#include "verbose.h"

static struct {
	int ret;
	int errnum;
} *shared;
#ifdef VALGRIND
int retpipe[2];
#endif

#ifdef VALGRIND
static const char *now(void)
{
	static char buf[31];
	static char *s = buf;

	if(s != buf)
	{
		free(s);
		s = buf;
	}

	struct timeval tv;
	if(gettimeofday(&tv, NULL) < 0)
	{
		perror("gettimeofday");
		abort();
	}

	size_t n = sizeof(buf);
	struct tm *t = localtime(&tv.tv_sec);
	while(strftime(s, n, "[%F %T.???%z]", t) == 0)
	{
		n = n * 3 / 2;
		if(s == buf)
			s = NULL;
		char *tmp = (char *)realloc(s, n);
		if(!tmp)
		{
			perror("realloc");
			free(s);
			abort();
		}
		s = tmp;
	}

	char ms[4];
	snprintf(ms, sizeof(ms), "%03u", (unsigned int)(tv.tv_usec / 1000));
	memcpy(strstr(s, "???"), ms, sizeof(ms) - 1);

	return s;
}
 #define record_start_test_fputs(x, y)       (printf("%s str=%d file=%2d\n", now(), (x).id, (y).id), record_start_test_fputs(x, y))
 #define record_timedout_test_fputs(x, y)    (record_timedout_test_fputs(), (void)printf("%s str=%d file=%2d\n\n", now(), (x).id, (y).id))
 #define record_ok_test_fputs(x, y, z)       (record_ok_test_fputs(z),      (void)printf("%s str=%d file=%2d\n\n", now(), (x).id, (y).id))
 #define record_crashed_test_fputs(x, y, z)  (record_crashed_test_fputs(z), (void)printf("%s str=%d file=%2d\n\n", now(), (x).id, (y).id))
 #define record_stopped_test_fputs(x, y, z)  (record_stopped_test_fputs(z), (void)printf("%s str=%d file=%2d\n\n", now(), (x).id, (y).id))
#else
 #define record_timedout_test_fputs(x, y)    record_timedout_test_fputs()
 #define record_ok_test_fputs(x, y, z)       record_ok_test_fputs(z)
 #define record_crashed_test_fputs(x, y, z)  record_crashed_test_fputs(z)
 #define record_stopped_test_fputs(x, y, z)  record_stopped_test_fputs(z)
#endif

FILE *generateFILE(int test_id)
{
	const char *mode = "wb";
	int         prot = 0;
	switch(test_id)
	{
	case TC_FILE_NULL:
	default:
		return NULL;
	case TC_FILE_RONLY:
		mode = "rb";
		break;
	case TC_FILE_WONLY:
		break;
	case TC_FILE_RW:
		mode = "r+b";
		break;
	case TC_FILE_CLOSED:
		break;
	case TC_FILE_MEM_RONLY:
	case TC_FILE_MEM_0_RONLY:
		prot = PROT_READ;
		break;
	case TC_FILE_MEM_WONLY:
	case TC_FILE_MEM_0_WONLY:
		prot = PROT_WRITE;
		break;
	case TC_FILE_MEM_RW:
	case TC_FILE_MEM_0_RW:
		prot = PROT_READ | PROT_WRITE;
		break;
	case TC_FILE_MEM_INACCESSABLE:
		prot = PROT_NONE;
		break;
	}

	FILE *fp = fopen("/dev/null", mode);
	if(!fp)
		return NULL;
	setbuf(fp, NULL);
	if(test_id == TC_FILE_CLOSED)
		return fclose(fp) == EOF ? NULL : fp;

#ifdef NOTOOLS
	size_t pagesize = sysconf(_SC_PAGE_SIZE);
	char *page;
#endif
	switch(test_id)
	{
	case TC_FILE_MEM_0_RONLY:
	case TC_FILE_MEM_0_WONLY:
	case TC_FILE_MEM_0_RW:
		memset(fp, 0, sizeof(*fp));
	case TC_FILE_MEM_RONLY:
	case TC_FILE_MEM_WONLY:
	case TC_FILE_MEM_INACCESSABLE:
#ifdef FILETOOSMALL
	case TC_FILE_MEM_RW:
#endif
#ifdef NOTOOLS
 #ifdef FILETOOSMALL
		page = (char *)mmap(NULL, 2 * pagesize, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if(page == MAP_FAILED)
		{
			int errbak = errno;
			fclose(fp);
			errno = errbak;
			return NULL;
		}
		munmap(page + pagesize, pagesize);
		memcpy(fp, page + pagesize - sizeof(*fp), sizeof(*fp));
 #else
		page = (char *)((uintptr_t)fp & ~(pagesize - 1));
 #endif
		if(mprotect(page, pagesize, prot) < 0)
		{
			int errbak = 0;
			fclose(fp);
			errno = errbak;
			return NULL;
		}
		if(test_id == TC_FILE_MEM_WONLY)
			memset(page, 0, pagesize);
		break;
#endif
#ifndef FILETOOSMALL
	case TC_FILE_MEM_RW:
 #ifdef NOTOOLS
		page = (char *)mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if(page == MAP_FAILED)
		{
			int errbak = 0;
			fclose(fp);
			errno = errbak;
			return NULL;
		}
		memcpy(page, fp, sizeof(*fp));
 #else
		fp = (FILE *)malloc_prot(sizeof(*fp), fp, prot);
 #endif
		break;
#endif
	}

	return fp;
}

const char *generateCSTR(int test_id)
{
	int prot;
	int guard = 0;
	switch(test_id)
	{
	case TC_CSTR_NULL:
	default:
		return NULL;
	case TC_CSTR_MEM_RONLY:
		prot = PROT_READ;
		break;
	case TC_CSTR_MEM_WONLY:
		prot = PROT_WRITE;
		break;
	case TC_CSTR_MEM_RW:
		prot = PROT_READ | PROT_WRITE;
		break;
	case TC_CSTR_MEM_0_RONLY:
		prot  = PROT_READ;
		guard = 1;
		break;
	case TC_CSTR_MEM_0_WONLY:
		prot  = PROT_WRITE;
		guard = 1;
		break;
	case TC_CSTR_MEM_0_RW:
		prot  = PROT_READ | PROT_WRITE;
		guard = 1;
		break;
	case TC_CSTR_MEM_INACCESSABLE:
		prot = PROT_NONE;
		break;
	}

#ifndef NOTOOLS
	char page[4096];
	size_t pagesize = sizeof(page);
#else
	size_t pagesize = sysconf(_SC_PAGE_SIZE);
	char *page = (char *)mmap(NULL, (1 + guard) * pagesize, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(page == MAP_FAILED)
		return NULL;
#endif

	switch(test_id)
	{
	case TC_CSTR_MEM_RONLY:
#ifndef NOTOOLS
	case TC_CSTR_MEM_WONLY:
#endif
	case TC_CSTR_MEM_RW:
		memset(page, ' ', pagesize - 1);
		page[pagesize - 1] = '\0';
		break;
	case TC_CSTR_MEM_0_RONLY:
#ifndef NOTOOLS
	case TC_CSTR_MEM_0_WONLY:
#endif
	case TC_CSTR_MEM_0_RW:
		memset(page, ' ', pagesize);
		break;
#ifdef NOTOOLS
	case TC_CSTR_MEM_WONLY:
	case TC_CSTR_MEM_0_WONLY:
		memset(page, 0, pagesize);
		break;
#endif
	}

#ifndef NOTOOLS
	return (char *)malloc_prot(pagesize, page, prot);
#else
	if(mprotect(page, pagesize, prot) < 0 || (guard && munmap(page + pagesize, pagesize) < 0))
	{
		int errbak = 0;
		munmap(page, pagesize);
		errno = errbak;
		return NULL;
	}

	return page;
#endif
}

static int test_subproc(int cstr_id, int file_id)
{
	const char *s = generateCSTR(cstr_id);
	if(!s && cstr_id != TC_CSTR_NULL)
	{
		perror("generateCSTR");
		return 2;
	}
	FILE *fp = generateFILE(file_id);
	if(!fp && file_id != TC_FILE_NULL)
	{
		perror("generateFILE");
		return 2;
	}

#ifdef KILL
	if(file_id == TC_FILE_MEM_RW && kill(getpid(), SIGSEGV) < 0)
	{
		perror("kill");
		return 2;
	}
#endif

	shared->ret = fputs(s, fp);
#ifdef FFLUSH
	if(shared->ret != EOF && fflush(fp) == EOF)
		shared->ret = EOF;
#endif
	shared->errnum = shared->ret == EOF ? errno : 0;

#ifdef VALGRIND
	ssize_t n = write(retpipe[1], shared, sizeof(*shared));
	if(n < 0)
	{
		perror("write");
		return 2;
	}
	else if(n != sizeof(*shared))
	{
		fputs("cannot write return value\n", stderr);
		return 2;
	}
#endif

	return shared->ret == EOF;
}

void test_fputs(const TestCase &str_testCase, const TestCase &file_testCase)
{
	record_start_test_fputs(str_testCase, file_testCase);

	pid_t child = fork();
	if(child < 0)
	{
		perror("fork");
		abort();
	}
	else if(child == 0)
	{
#ifdef VALGRIND
		char *argv[6];
		char *args;
		if(asprintf(&args, "%d%c%d%c%d", retpipe[1], 0, str_testCase.id, 0, file_testCase.id) < 0)
		{
			perror("asprintf");
			_exit(2);
		}

		argv[0] = "valgrind";
		argv[1] = "./fputs_test";
		argv[2] = args;
		argv[3] = argv[2] + strlen(argv[2]) + 1;
		argv[4] = argv[3] + strlen(argv[3]) + 1;
		argv[5] = NULL;

		execvp(argv[0], argv);
		perror("execvp");
		_exit(2);
#else
		_exit(test_subproc(str_testCase.id, file_testCase.id));
#endif
	}

	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	struct timespec t = {2, 0};
	sigtimedwait(&set, NULL, &t);

	int status;
	pid_t pid = waitpid(child, &status, WNOHANG);
	if(pid != child)
	{
		if(pid == 0)
		{
			if(kill(child, SIGKILL) < 0)
			{
				perror("kill");
				abort();
			}
			record_timedout_test_fputs(str_testCase, file_testCase);
			if(waitpid(child, &status, 0) == child)
			{
				test_results[file_testCase.id][str_testCase.id].result = RES_TIMEOUT;
				return;
			}
		}
		perror("waitpid");
		abort();
	}

	if(WIFEXITED(status))
	{
		if(WEXITSTATUS(status) == 2)
		{
			fputs("cannot initialize test\n", stderr);
			abort();
		}

#ifdef VALGRIND
		ssize_t n = read(retpipe[0], shared, sizeof(*shared));
		if(n < 0)
		{
			perror("read");
			abort();
		}
		else if(n != sizeof(*shared))
		{
			fputs("cannot read return value\n", stderr);
			abort();
		}
#endif

		test_results[file_testCase.id][str_testCase.id].result  = RES_OK;
		test_results[file_testCase.id][str_testCase.id].data[0] = shared->ret;
		test_results[file_testCase.id][str_testCase.id].data[1] = shared->errnum;
		record_ok_test_fputs(str_testCase, file_testCase, WEXITSTATUS(status) ? EOF : 1);
	}
	else if(WIFSIGNALED(status))
	{
		test_results[file_testCase.id][str_testCase.id].result  = RES_CRASH;
		test_results[file_testCase.id][str_testCase.id].data[0] = WTERMSIG(status);
		record_crashed_test_fputs(str_testCase, file_testCase, WTERMSIG(status));
	}
	else if(WIFSTOPPED(status))
	{
		test_results[file_testCase.id][str_testCase.id].result  = RES_STOP;
		test_results[file_testCase.id][str_testCase.id].data[0] = WTERMSIG(status);
		record_stopped_test_fputs(str_testCase, file_testCase, WSTOPSIG(status));
	}
	else
	{
		fputs("this piece of code should not be reached\n", stderr);
		abort();
	}
}

void sigchld(int signum)
{
	(void)signum;
	return;
}

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);

#ifdef VALGRIND
	static typeof(*shared) x;
	shared = &x;

	if(argc == 4)
	{
		char *end;
		unsigned long l;
		int test_str, test_file;

		if((l = strtoul(argv[1], &end, 0)) > INT_MAX || *end)
		{
			fprintf(stderr, "invalid errno file descriptor: %s\n", argv[1]);
			return 2;
		}
		retpipe[1] = l;

		if((l = strtoul(argv[2], &end, 0)) > testCases_CSTR_count || *end)
		{
			fprintf(stderr, "invalid string test case: %s\n", argv[2]);
			return 2;
		}
		test_str = l;

		if((l = strtoul(argv[3], &end, 0)) > testCases_FILE_count || *end)
		{
			fprintf(stderr, "invalid file test case: %s\n", argv[3]);
			return 2;
		}
		test_file = l;

		return test_subproc(test_str, test_file);
	}
#else
	(void)argc, (void)argv;
#endif

#ifdef VALGRIND
	if(pipe(retpipe) < 0)
	{
		perror("pipe");
		abort();
	}
#else
	shared = (typeof(shared))mmap(NULL, sizeof(*shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(shared == MAP_FAILED)
	{
		perror("mmap");
		abort();
	}
#endif

	signal(SIGCHLD, &sigchld);

	for(int i = 0; i < testCases_FILE_count; i++)
		for(int j = 0; j < testCases_CSTR_count; j++)
			test_fputs(testCases_CSTR[j], testCases_FILE[i]);
	print_summary();

	print_results(NULL, stdout);

	return 0;
}
