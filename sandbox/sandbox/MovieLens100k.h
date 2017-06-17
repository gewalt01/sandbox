#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "CFDataset.h"

class MovieLens100k
{
public:
	MovieLens100k() = default;
	~MovieLens100k() = default;
	CFDataset readDataset(const std::string & fname);
};

inline CFDataset MovieLens100k::readDataset(const std::string & fname)
{
	size_t users = 943;
	size_t items = 1682;

	CFDataset cfd(users, items);

	std::ifstream ifs(fname, std::ios::in);

	while (!ifs.eof())
	{
		int user_id;
		int item_id;
		int rating;

		char s_buff[0xFF];
		ifs.getline(s_buff, 0xFF);
		std::string line_str(s_buff);
		std::stringstream ss(line_str);

		ss >> user_id >> item_id >> rating;
		cfd.existDataPoint[user_id - 1][item_id - 1] = 1;
		cfd.dataset[user_id - 1][item_id - 1] = rating - 1;
	}

	return cfd;
}
