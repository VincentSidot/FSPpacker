
#include <iostream>
#include <initializer_list>
#include "Matrix.h"

int main(int argc, char** argv)
{
	
	Matrix::Matrix < 5, 5, int > test(0);
	test.fill(	{ 1, 2, 3, 4, 5, 6 });
	test.disp();
	
	std::cout << "\n";
	test.disp();
	std::cout << "\n";

	std::cin.get();
	return 0;


}