#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if(argc != 3)
		return 2;

	int fd, fdr;
	if((fd = open(argv[1], O_RDONLY)) < 0 || (fdr = open(argv[2], O_RDONLY)) < 0)
	{
		perror("open");
		return 2;
	}

	struct stat st;
	if(fstat(fdr, &st) < 0)
	{
		perror("fstat");
		return 2;
	}

	char   buf[2][4096];
	size_t buflen[] = {0, 0};
	char  *p[]      = {buf[0], buf[1]};

	off_t off = st.st_size;
	off = (off + sizeof(buf[0]) - 1) & ~(sizeof(buf[0]) - 1);

	int eq = 1;
	do
	{
		int ended = 0;

		if(p[0] == buf[0] + buflen[0])
		{
			ssize_t m = read(fd, buf[0], sizeof(buf[0]));
			if(m < 0)
			{
				perror("read");
				return 2;
			}
			if(m == 0)
				ended = 1;
			p[0]      = buf[0];
			buflen[0] = m;
		}

		if(p[1] == buf[1])
		{
			if(off == 0)
			{
				eq = ended;
				break;
			}
			off -= 4096;

			if(lseek(fdr, off, SEEK_SET) != off)
			{
				perror("lseek");
				return 2;
			}

			ssize_t m = read(fdr, buf[1], sizeof(buf[1]));
			if(m != 4096 && m != (st.st_size - off))
			{
				perror("read");
				return 2;
			}
			p[1]      = buf[1] + m;
			buflen[1] = m;
		}
		else if(ended)
		{
			eq = 0;
			break;
		}

		eq = *p[0]++ == *--p[1];
	}
	while(eq);

	close(fd);
	close(fdr);

	if(!eq)
		fputs("files differ\n", stdout);

	return eq ? 0 : 1;
}
