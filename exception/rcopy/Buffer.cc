#include "Buffer.h"
#include "Exception.h"
#include <cerrno>
#include <cstdlib>

Buffer::Buffer(int n)
{
	if(n < 0)
		throw Exception(EOVERFLOW);
	this->ptr = 0;
	this->len = 0;
	this->resize(n);
}

Buffer::Buffer(size_t n)
{
	this->ptr = 0;
	this->len = 0;
	this->resize(n);
}

Buffer::~Buffer(void)
{
	free(this->ptr);
}

void Buffer::grow(size_t n)
{
	if(n > this->len)
		this->resize(n);
}

void Buffer::resize(size_t n)
{
	if(n == 0)
	{
		free(this->ptr);
		this->ptr = 0;
	}
	else
	{
		void *tmp = realloc(this->ptr, n);
		if(!tmp)
			throw Exception();
		this->ptr = tmp;
		this->len = n;
	}
}

void Buffer::reverse(size_t n)
{
	if(n > this->len)
		n = this->len;
	for(char *a = (char *)this->ptr, *z = a + n; a < --z; a++)
	{
		char tmp = *a;
		*a = *z;
		*z = tmp;
	}
}
