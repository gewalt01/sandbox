#include "Initializer.h"
#include <iostream>
#include <vector>

std::vector<int> test(std::vector<int> vect)
{
	return vect;
}

int main(void)
{

	auto a = test({ 1,2,3,4,5 });
	return 0;
}