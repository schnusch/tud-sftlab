#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <sys/types.h>

class Buffer
{
public:
	Buffer(int size);
	friend class File;

	Buffer(size_t n);
	~Buffer(void);
	void grow(size_t n);
	void resize(size_t n);
	void reverse(size_t n);

private:
	void *ptr;
	size_t len;
};

#endif
