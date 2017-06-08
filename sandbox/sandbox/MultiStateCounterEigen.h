#pragma once
#include <Eigen/Core>
class MultiStateCounterEigen
{
protected:
	Eigen::VectorXi _stateTable;
	Eigen::VectorXi _countTable;
	Eigen::VectorXi _cycleTable;
	Eigen::VectorXi _quantumTable;


public:
	MultiStateCounterEigen();
	MultiStateCounterEigen(Eigen::VectorXi & states);
	~MultiStateCounterEigen();

	int getCycleCount();
	Eigen::VectorXi getCounterTable();
	void next();
};

MultiStateCounterEigen::MultiStateCounterEigen()
{
	throw;
}


MultiStateCounterEigen::MultiStateCounterEigen(Eigen::VectorXi & states)
{
	this->_stateTable = states;
	this->_countTable = states;
	this->_countTable.setConstant(0);
	this->_cycleTable = states;
	this->_quantumTable = states;

	this->_cycleTable(0) = this->_stateTable(0);
	this->_quantumTable(0) = 1;
	for (int i = 1; i < this->_cycleTable.size(); i++)
	{
		this->_cycleTable(i) = this->_stateTable(i) * this->_cycleTable(i - 1);
		this->_quantumTable(i) = this->_cycleTable(i - 1);
	}
}

MultiStateCounterEigen::~MultiStateCounterEigen()
{

}

int MultiStateCounterEigen::getCycleCount()
{
	return this->_cycleTable(this->_cycleTable.size()-1);
}

Eigen::VectorXi MultiStateCounterEigen::getCounterTable()
{
	auto counter = this->_countTable;
	for (int i = 0; i < counter.size(); i++)
	{
		counter(i) /= this->_quantumTable(i);
	}
	return counter;
}

void MultiStateCounterEigen::next()
{
	for (int i = 0; i < this->_stateTable.size(); i++)
	{
		this->_countTable(i) = (this->_countTable(i) + 1) % this->_cycleTable(i);
	}
}

/*

#include "MultiStateCounterEigen.h"
#include <iostream>
#include <vector>

int main(void)
{
std::vector<int> v = { 2,3,2,4 };
Eigen::VectorXi eig = Eigen::Map<Eigen::VectorXi>(v.data(), v.size());
MultiStateCounterEigen msc(eig);


for (int i = 0; i < 2 * msc.getCycleCount(); i++)
{
auto count = msc.getCounterTable();
for (int i = count.size()-1; 0 <= i; i--)
{
std::cout << count[i] << ",";
}
std::cout << std::endl;
msc.next();
}
}

*/