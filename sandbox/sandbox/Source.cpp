#include "CFDataset.h"
#include "MovieLens100k.h"
#include <iostream>
#include <string>

int main(void)
{
	MovieLens100k ml;
	auto dataset = ml.readDataset(std::string("ml-100k/ua.base"));

	std::string str;
	std::cin >> str;
	return 0;
}