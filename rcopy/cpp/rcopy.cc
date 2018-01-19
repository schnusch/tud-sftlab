#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

class File
{
public:
	File(const char *name, int flags)
	{
		if(flags & O_CREAT)
			this->fd = ::open(name, flags, 0666);
		else
			this->fd = ::open(name, flags);
		if(this->fd < 0)
		{
			std::perror("open");
			std::exit(1);
		}
	}

	size_t read(char *buf, size_t n)
	{
		ssize_t m = ::read(this->fd, buf, n);
		if(m < 0)
		{
			std::perror("read");
			std::exit(1);
		}
		return m;
	}

	void write(const char *buf, size_t n)
	{
		ssize_t m = 0;
		while(n > 0 && (m = ::write(this->fd, buf, n)) > 0)
			buf += m, n -= m;
		if(m < 0)
		{
			std::perror("write");
			std::exit(1);
		}
	}

	off_t length(void)
	{
		struct stat st;
		if(::fstat(this->fd, &st) < 0)
		{
			std::perror("fstat");
			std::exit(1);
		}
		return st.st_size;
	}

	void seek(off_t off)
	{
		if(::lseek(this->fd, off, SEEK_SET) < 0)
		{
			std::perror("lseek");
			std::exit(1);
		}
	}

	~File()
	{
		if(this->fd >= 0)
			::close(this->fd);
	}

private:
	int fd;
};

int main(int argc, char **argv)
{
	if(argc != 3)
		return 2;

	File in( argv[1], O_RDONLY);
	File out(argv[2], O_WRONLY | O_TRUNC | O_CREAT);

	char buf[4096];
	off_t len = in.length();
	off_t off = len - (len % sizeof(buf));
	while(1)
	{
		in.seek(off);
		size_t n = in.read(buf, sizeof(buf));
		if(n > 0)
		{
			for(int a = 0, z = n; a < --z; a++)
			{
				char tmp = buf[a];
				buf[a] = buf[z];
				buf[z] = tmp;
			}
			out.write(buf, n);
		}
		if(off == 0)
			break;
		off -= sizeof(buf);
	}

	return 0;
}
