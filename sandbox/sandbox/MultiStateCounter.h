#pragma once
#include <vector>


class MultiStateCounter
{
protected:
	std::vector<int> _stateTable;
	std::vector<int> _countTable;
	std::vector<int> _cycleTable;
	std::vector<int> _quantumTable;

public:
	MultiStateCounter();
	MultiStateCounter(std::vector<int> & states);
	~MultiStateCounter();

	int getCycleCount();
	std::vector<int> getCounterTable();
	void next();
};

MultiStateCounter::MultiStateCounter()
{
	throw;
}

MultiStateCounter::MultiStateCounter(std::vector<int> & states)
{
	this->_stateTable = states;
	this->_countTable = std::vector<int>(states.size(), 0);
	this->_cycleTable = states;
	this->_quantumTable = states;


	this->_cycleTable[0] = this->_stateTable[0];
	this->_quantumTable[0] = 1;
	for (int i = 1; i < this->_cycleTable.size(); i++)
	{
		this->_cycleTable[i] = this->_stateTable[i] * this->_cycleTable[i - 1];
		this->_quantumTable[i] = this->_cycleTable[i - 1];
	}
}

MultiStateCounter::~MultiStateCounter()
{

}

int MultiStateCounter::getCycleCount()
{
	return this->_cycleTable.back();
}

std::vector<int> MultiStateCounter::getCounterTable()
{
	auto counter = this->_countTable;
	for (int i = 0; i < counter.size(); i++)
	{
		counter[i] /= this->_quantumTable[i];
	}

	return counter;
}

void MultiStateCounter::next()
{
	for (int i = 0; i < this->_stateTable.size(); i++)
	{
		this->_countTable[i] = (this->_countTable[i] + 1) % this->_cycleTable[i];
	}
}