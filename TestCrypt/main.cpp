
#include <iostream>
#include <initializer_list>
#include "Matrix.h"

int main(int argc, char** argv)
{
	
	Matrix::Matrix < 5, 5, float > test(0);
	test.fill({ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24});
	test.disp();
	test = Matrix::reverse(test);
	std::cout << "\n";
	test.disp();

	std::cin.get();
	return 0;


}