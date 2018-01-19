#include "Exception.h"
#include "File.h"
#include "io.h"

#include <cerrno>
#include <sys/stat.h>

File::File(const char *name, const char *mode)
{
	this->fp = fopen(name, mode);
	this->is_open();
}

File::~File(void)
{
	try
	{
		this->close();
	}
	catch(Exception e) {}
}

void File::is_open(void)
{
	if(!this->fp)
		throw Exception();
}

void File::close(void)
{
	this->is_open();
	if(fclose(this->fp) == EOF)
		throw Exception();
	this->fp = NULL;
}

off_t File::size(void)
{
	this->is_open();
	struct stat st;
	int fd;
	if((fd = fileno(this->fp)) < 0 || fstat(fd, &st) < 0)
		throw Exception();
	return st.st_size;
}

int File::read(Buffer &b, int n)
{
	if(n < 0)
		throw Exception(EOVERFLOW);
	return this->read(b, (size_t)n);
}

void File::write(Buffer &b, int n)
{
	if(n < 0)
		throw Exception(EOVERFLOW);
	this->write(b, (size_t)n);
}

void File::seek(int pos)
{
	if(pos < 0)
		pos = 0;
	this->seek((off_t)pos);
}

size_t File::read(Buffer &b, size_t n)
{
	this->is_open();
	if(n > b.len)
		throw Exception(EINVAL);
	if(freadall(this->fp, (char *)b.ptr, &n) < 0)
		throw Exception();
	return n;
}

void File::write(Buffer &b, size_t n)
{
	this->is_open();
	if(n > b.len)
		throw Exception(EINVAL);
	if(fwriteall(this->fp, (const char *)b.ptr, &n) < 0)
		throw Exception();
}

void File::seek(off_t off) { this->seek(off, SEEK_SET); }
void File::seek(off_t off, int whence)
{
	this->is_open();
	if(whence == SEEK_SET && off > this->size())
		throw Exception(EINVAL);
	if(fseeko(this->fp, off, whence) < 0)
		throw Exception();
}
