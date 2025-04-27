#include "PolynomialMap.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

#define EPSILON 1.0e-10 // zero double

using namespace std;

PolynomialMap::PolynomialMap(const PolynomialMap &other)
{
	m_Polynomial = other.m_Polynomial;
}

PolynomialMap::PolynomialMap(const string &file)
{
	ReadFromFile(file);
}

PolynomialMap::PolynomialMap(const double *cof, const int *deg, int n)
{
	for (int i = 0; i < n; i++)
	{
		AddOneTerm(deg[i], cof[i]);
	}
}

PolynomialMap::PolynomialMap(const vector<int> &deg, const vector<double> &cof)
{
	assert(deg.size() == cof.size());
	for (int i = 0; i < deg.size(); i++)
	{
		AddOneTerm(deg[i], cof[i]);
	}
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
	auto itr = m_Polynomial.begin();
	while (itr != m_Polynomial.end())
	{
		if (fabs(itr->second) < EPSILON)
			itr = m_Polynomial.erase(itr);
		else
			++itr;
	}
}

PolynomialMap PolynomialMap::operator+(const PolynomialMap &right) const
{
	PolynomialMap poly(*this);
	for (const auto &term : right.m_Polynomial)
	{
		poly.AddOneTerm(term.first, term.second);
	}
	poly.compress();
	return poly;
}

PolynomialMap PolynomialMap::operator-(const PolynomialMap &right) const
{
	PolynomialMap poly(*this);
	for (const auto &term : right.m_Polynomial)
	{
		poly.AddOneTerm(term.first, -term.second);
	}
	poly.compress();
	return poly;
}

PolynomialMap PolynomialMap::operator*(const PolynomialMap &right) const
{
	PolynomialMap poly(*this);
	for (const auto &term1 : poly.m_Polynomial)
	{
		for (const auto &term2 : right.m_Polynomial)
		{
			double cof = term1.second * term2.second;
			int deg = term1.first + term2.first;
			poly.AddOneTerm(deg, cof);
		}
	}
	poly.compress();
	return poly;
}

PolynomialMap &PolynomialMap::operator=(const PolynomialMap &right)
{
	m_Polynomial = right.m_Polynomial;
	return *this;
}

void PolynomialMap::Print() const
{
	bool firstTerm = true;
	for (const auto &term : m_Polynomial)
	{
		if (!firstTerm && term.second > 0)
			cout << "+";

		cout << term.second;
		if (term.first > 0)
			cout << "x^" << term.first;

		firstTerm = false;
	}
	cout << endl;
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
		AddOneTerm(deg, cof);
	}

	fin.close();

	return true; // you should return a correct value
}

void PolynomialMap::AddOneTerm(int deg, double cof)
{
	auto itr = m_Polynomial.find(deg);
	if (itr == m_Polynomial.end())
	{
		m_Polynomial[deg] = cof;
	}
	else
	{
		itr->second += cof;
		if (fabs(itr->second) < EPSILON)
			m_Polynomial.erase(itr);
	}
}