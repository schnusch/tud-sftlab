#include <iostream>
#include "File.h"
#include "Buffer.h"
#include "Exception.h"

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <file> <rfile>" << std::endl;
		return 2;
	}

	try
	{
		File fin (argv[1], "rb");
		File fout(argv[2], "wb");

		Buffer b(4096);
		off_t end = fin.size();
		while(end > 0)
		{
			size_t n = end < 4096 ? end : 4096;
			off_t start = end - n;
			fin.seek(start);
			if(fin.read(b, n) != n)
			{
				std::cerr << argv[0] << ": short read" << std::endl;
				return 1;
			}
			b.reverse(n);
			fout.write(b, n);
			end = start;
		}

		fin.close();
		fout.close();
	}
	catch(Exception e)
	{
		std::cerr << argv[0] << ": " << e.getMessage() << std::endl;
		return 1;
	}

	return 0;
}
