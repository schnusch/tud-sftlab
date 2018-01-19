#include "Exception.h"
#include <cerrno>
#include <string.h>

Exception::Exception(void)
{
	this->errnum = errno;
}

Exception::Exception(int errnum)
{
	this->errnum = errnum;
}

const char *Exception::getMessage(void)
{
	return strerror(this->errnum);
}

int Exception::getErrno(void)
{
	return this->errnum;
}
