#include <iostream>
#include <string>
#include <Windows.h>
#include "packer.h"


int main(int argc, char** argv)
{
	std::wstring path;
	std::wcout << "Enter file path : ";
	std::wcin >> path;
	WORD rep = convertToPacker(path.c_str(), true);
	if (rep == SUCCESS)
	{
		std::cout << "Success\n";
	}
	else
	{
		std::cout << "Error : " << rep << std::endl;
	}
	return 0;
}