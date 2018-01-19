#ifndef __EXECPTION_H__
#define __EXECPTION_H__

class Exception
{
public:
	Exception(void);
	Exception(int errnum);
	const char* getMessage();
	int getErrno();
private:
	int errnum;
};

#endif
