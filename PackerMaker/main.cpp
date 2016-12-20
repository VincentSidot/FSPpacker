#include <iostream>
#include <string>
#include <Windows.h>
#include "packer.h"


int main(int argc, char** argv)
{
	std::wstring path;
	std::wcout << "Enter file path : ";
	std::wcin >> path;
	std::cin.get();
	std::cout << "Would you make backup ?(Y/N)\n";
	char car = std::cin.get();
	WORD rep = convertToPacker(path.c_str(), (car == 'y' || car == 'Y')?true:false);
	if (rep == SUCCESS)
	{
		std::cout << "Success :) \n";
	}
	else if (rep == ERROR_ALREADY_PACKAGE)
	{
		std::cout << "File already packed ;)\n";
	}
	else
	{
		std::cout << "Error : " << rep << " :'(\n";
	}
	std::cin.get();
	std::cin.get();
	return 0;
}