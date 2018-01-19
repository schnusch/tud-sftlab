#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if(argc < 2 || 3 < argc)
	{
		printf("Usage: %s <file> <rfile>\n"
				"       %s <file>\n", argv[0], argv[0]);
		return 2;
	}

	int fdin, fdout;
	if((fdin = open(argv[1], O_RDONLY)) < 0 || (fdout = argc == 2 ? STDOUT_FILENO : open(argv[2], O_RDWR | O_TRUNC | O_CREAT, 0666)) < 0)
	{
		perror("open");
		return 1;
	}

	struct stat st;
	if(fstat(fdin, &st) < 0)
	{
		perror("stat");
		return 1;
	}
	if(st.st_size == 0)
		return 0;

	size_t pgsize = sysconf(_SC_PAGE_SIZE);
	off_t  off    = (st.st_size - 1) & ~(pgsize - 1);
	size_t len    = st.st_size - off;
	for(; off >= 0; off -= pgsize)
	{
		char *map;
		if((map = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdin, off)) == MAP_FAILED)
		{
			perror("mmap");
			return 1;
		}

		for(char *a = map, *z = map + len; a < z--; a++)
		{
			char tmp = *a;
			*a = *z;
			*z = tmp;
		}

		const char *p = map;
		size_t      n = len;
		while(n > 0)
		{
			ssize_t m = write(fdout, p, n);
			if(m < 0)
			{
				perror("write");
				return 1;
			}
			p += m;
			n -= m;
		}

		if(munmap(map, len) < 0)
		{
			perror("munmap");
			return 1;
		}

		len = 4096;
	}

	close(fdin);
	close(fdout);

	return 0;
}
