#pragma once
#include <vector>
#include <iostream>
class SplitTongueEigen
{
protected:
	double max = 0;
	double min = 0;

public://accesser
	double getMax();
	double getMin();
	void setMax(double max);
	void setMin(double min);

public://open method
	SplitTongueEigen();
	SplitTongueEigen(double min, double max);
	~SplitTongueEigen() = default;

	std::vector<double> split(int n);
	static std::vector<double> split(double min, double max, int n);
};

double SplitTongueEigen::getMax()
{
	return this->max;
}

double SplitTongueEigen::getMin()
{
	return this->min;
}

void SplitTongueEigen::setMax(double max)
{
	if (this->min > max) return;

	this->max = max;
}

void SplitTongueEigen::setMin(double min)
{
	if (min > this->max) return;

	this->min = min;
}

SplitTongueEigen::SplitTongueEigen()
{
	throw;
}

SplitTongueEigen::SplitTongueEigen(double min, double max)
{
	if (max <= min) throw;
	this->min = min;
	this->max = max;
}

std::vector<double> SplitTongueEigen::split(double min, double max, int n)
{
	std::vector<double> vect(n + 1);

	for (int i = 0; i <= n; i++)
	{
		vect[i] = min + (max - min) / (double)n * i;
	}

	return vect;
}


std::vector<double> SplitTongueEigen::split(int n)
{
	return this->split(this->min, this->max, n);
}

