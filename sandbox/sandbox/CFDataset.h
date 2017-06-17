#pragma once
#include <vector>

class CFDataset {
public:
	size_t userSize;
	size_t itemSize;

	std::vector< std::vector<int> > existDataPoint;
	std::vector< std::vector<int> > dataset;
public:
	CFDataset() = default;
	CFDataset(size_t & user_size, size_t & item_size);
	~CFDataset() = default;
};

inline CFDataset::CFDataset(size_t & user_size, size_t & item_size)
{
	this->userSize = user_size;
	this->itemSize = item_size;

	this->existDataPoint = std::vector< std::vector<int> >(user_size);
	for (auto & user : existDataPoint) user = std::vector<int>(item_size, 0);

	this->dataset = std::vector< std::vector<int> >(user_size);
	for (auto & data : dataset) data = std::vector<int>(item_size, 0);
}
