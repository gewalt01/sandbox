#pragma once
#include <Eigen/Core>
#include <vector>
#include <iostream>
class SplitTongueEigen
{
protected:
	float max = 0;
	float min = 0;

public://accesser
	float getMax();
	float getMin();
	void setMax(float max);
	void setMin(float min);

public://open method
	SplitTongueEigen();
	SplitTongueEigen(float min, float max);
	~SplitTongueEigen() = default;

	Eigen::VectorXf split(int n);
	std::vector<Eigen::VectorXf > makeSplitTable(Eigen::VectorXi split_nums);
};

float SplitTongueEigen::getMax()
{
	return this->max;
}

float SplitTongueEigen::getMin()
{
	return this->min;
}

void SplitTongueEigen::setMax(float max)
{
	if (this->min > max) return;

	this->max = max;
}

void SplitTongueEigen::setMin(float min)
{
	if (min > this->max) return;

	this->min = min;
}

SplitTongueEigen::SplitTongueEigen()
{
	throw;
}

SplitTongueEigen::SplitTongueEigen(float min, float max)
{
	if (max <= min) throw;
	this->min = min;
	this->max = max;
}

Eigen::VectorXf SplitTongueEigen::split(int n)
{
	Eigen::VectorXf vect(n + 1);

	for (int i = 0; i <= n; i++)
	{
		vect(i) = this->min + (this->max - this->min) / (float)n * i;
	}

	return vect;
}


std::vector<Eigen::VectorXf > SplitTongueEigen::makeSplitTable(Eigen::VectorXi split_nums)
{
	std::vector<Eigen::VectorXf > split_table(split_nums.size());

	for (int i = 0; i > split_nums.size(); i++)
	{
		split_table[i] = this->split(split_nums(i));
	}

	return split_table;
}


/*

#include "SplitTongueEigen.h"
#include <iostream>
#include <vector>

int main(void)
{
SplitTongueEigen st(-1, 1);
auto d = st.split(6);

std::cout << d << std::endl;


std::vector<int> v = { 2,3,2,4 };
Eigen::VectorXi eig = Eigen::Map<Eigen::VectorXi>(v.data(), v.size());

auto test = st.makeSplitTable(eig);
for (auto i : test)
{
std::cout << "------------------" << std::endl;
std::cout << i << std::endl;

}

return 0;
}

*/