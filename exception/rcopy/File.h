#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>

#include "Buffer.h"

class File
{
public:
	File(const char* name, const char *mode);
	int read(Buffer& buffer, int size);
	void write(Buffer& buffer, int size);
	void seek(int pos);
	void close();

	~File(void);
	void is_open(void);
	off_t size(void);
	size_t read(Buffer& buffer, size_t size);
	void write(Buffer& buffer, size_t size);
	void seek(off_t pos);
	void seek(off_t pos, int whence);

private:
	FILE *fp;
};

#endif
