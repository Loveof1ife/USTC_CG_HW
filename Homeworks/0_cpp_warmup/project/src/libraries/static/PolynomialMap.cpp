#include "PolynomialMap.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

using namespace std;

PolynomialMap::PolynomialMap(const PolynomialMap &other)
{
	// TODO
}

PolynomialMap::PolynomialMap(const string &file)
{
	ReadFromFile(file);
}

PolynomialMap::PolynomialMap(const double *cof, const int *deg, int n)
{
	// TODO
}

PolynomialMap::PolynomialMap(const vector<int> &deg, const vector<double> &cof)
{
	assert(deg.size() == cof.size());
	// TODO
}

double PolynomialMap::coff(int i) const
{
	auto target = m_Polynomial.find(i);
	if (target == m_Polynomial.end())
		return 0.f;

	return target->second;
}

double &PolynomialMap::coff(int i)
{
	return m_Polynomial[i];
}

void PolynomialMap::compress()
{
	// TODO
}

PolynomialMap PolynomialMap::operator+(const PolynomialMap &right) const
{
	// TODO
	return {}; // you should return a correct value
}

PolynomialMap PolynomialMap::operator-(const PolynomialMap &right) const
{
	// TODO
	return {}; // you should return a correct value
}

PolynomialMap PolynomialMap::operator*(const PolynomialMap &right) const
{
	// TODO
	return {}; // you should return a correct value
}

PolynomialMap &PolynomialMap::operator=(const PolynomialMap &right)
{
	// TODO
	return *this;
}

void PolynomialMap::Print() const
{
	// TODO
}

bool PolynomialMap::ReadFromFile(const string &file)
{
	m_Polynomial.clear();
	ifstream fin(file);
	if (!fin)
	{
		cerr << "Error: Cannot open file " << file << endl;
		return false;
	}
	char ch;
	int n;
	fin >> ch >> n;

	for (int i = 0; i < n; i++)
	{
		int deg;
		double cof;
		fin >> deg >> cof;
		coff[deg] = cof;
	}

	fin.close();

	return true; // you should return a correct value
}
