#pragma once
#include <initializer_list>
#include <Eigen/Core>

class Initializer {
	
public:
	Eigen::VectorXi vect;

public:
	Initializer();
	template<class T>
	Initializer(std::initializer_list<T> a);
	~Initializer();

};


Initializer::Initializer()
{
	throw;
}

template<class T>
Initializer::Initializer(std::initializer_list<T> a)
{
	this->vect = Eigen::Map<Eigen::VectorXi>(a.);
}

Initializer::~Initializer()
{

}
