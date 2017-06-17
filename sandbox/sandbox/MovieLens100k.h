#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class MovieLens100k
{
public:
	std::vector< std::vector<int> > existDataPoint;
	std::vector< std::vector<int> > dataset;

public:
	MovieLens100k() = default;
	MovieLens100k(const std::string & fname);
	~MovieLens100k() = default;
	void readDataset(const std::string & fname);
};

inline MovieLens100k::MovieLens100k(const std::string & fname)
{
	int users = 943;
	int items = 1682;

	this->existDataPoint = std::vector< std::vector<int> >(users);
	for (auto & user : existDataPoint) user = std::vector<int>(items, 0);

	this->dataset = std::vector< std::vector<int> >(users);
	for (auto & data : dataset) data = std::vector<int>(items, 0);

	this->readDataset(fname);
}

inline void MovieLens100k::readDataset(const std::string & fname)
{
	int users = 943;
	int items = 1682;

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
		this->existDataPoint[user_id - 1][item_id - 1] = 1;
		this->dataset[user_id - 1][item_id - 1] = rating - 1;
	}

}
