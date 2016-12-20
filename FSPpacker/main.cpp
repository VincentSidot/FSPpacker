#include "data.h"
#include "runpe.h"
#include <iostream>

int main(int argc, char** argv)
{
	data file = getData();
	if (file.error)
	{
		std::cout << "Error couldn't get information from app" << std::endl;
		std::cin.get();
		return -1;
	}
	std::cout << "Successfull unpack" << std::endl;
	RunPortableExecutable(file.buffer);
	return 0;
}