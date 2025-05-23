// implementation of class DArray
#include "DArray.h"

// default constructor
DArray::DArray()
{
	Init();
}

// set an array with default values
DArray::DArray(int nSize, double dValue)
	: m_nSize(nSize), m_nMax(nSize * 2), m_pData(new double[m_nMax])
{
	for (int i = 0; i < nSize; i++)
		m_pData[i] = dValue;
}

DArray::DArray(const DArray &arr)
	: m_pData(new double[arr.m_nMax]), m_nSize(arr.m_nSize), m_nMax(arr.m_nMax)
{
	// copy constructor
	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr.m_pData[i];
}

// deconstructor
DArray::~DArray()
{
	Free();
}

// display the elements of the array
void DArray::Print() const
{
	std::cout << "size= " << m_nSize << ":";
	for (int i = 0; i < m_nSize; i++)
	{
		if (i != 0)
			std::cout << ", ";
		std::cout << m_pData[i];
	}
	std::cout << std::endl;
}

// initilize the array
void DArray::Init()
{
	m_pData = nullptr;
	m_nSize = 0;
	m_nMax = 0;
}

// free the array
void DArray::Free()
{
	if (m_pData != nullptr)
	{
		delete[] m_pData;
		m_pData = nullptr;
	}
	m_nSize = 0;
	m_nMax = 0;
}

// get the size of the array
int DArray::GetSize() const
{
	return m_nSize;
}

// set the size of the array
void DArray::SetSize(int nSize)
{
	if (nSize == m_nSize)
		return;
	Reserve(nSize);
	for (int i = m_nSize; i < nSize; i++)
		m_pData[i] = 0.;
	m_nSize = nSize;
}

void DArray::Reserve(int nSize)
{
	if (m_nMax >= nSize)
		return; // already have enough memory

	while (m_nMax < nSize)
		m_nMax = m_nMax == 0 ? 1 : 2 * m_nMax; // double the size

	double *pData = new double[m_nMax];
	memcpy(pData, m_pData, m_nSize * sizeof(double));

	delete[] m_pData; // free old memory
	m_pData = pData;  // set the pointer to the new memory
}

// get an element at an index
const double &DArray::GetAt(int nIndex) const
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// set the value of an element
void DArray::SetAt(int nIndex, double dValue)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = dValue;
}

// overload operator '[]'
double &DArray::operator[](int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex]; // you should return a correct value
}

// overload operator '[]'
const double &DArray::operator[](int nIndex) const
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex]; // you should return a correct value
}

// add a new element at the end of the array
void DArray::PushBack(double dValue)
{
	Reserve(m_nSize + 1);
	m_pData[m_nSize] = dValue;
	m_nSize++;
	std::cout << "PushBack: " << dValue << std::endl;
}

// delete an element at some index
void DArray::DeleteAt(int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	double *pTemp = new double[m_nMax];
	for (int i = nIndex + 1; i < m_nSize; i++)
	{
		pTemp[i - 1] = m_pData[i]; // copy the elements after nIndex
	}
	m_nSize--;
	std::cout << "DeleteAt: " << nIndex << std::endl;
}

// insert a new element at some index
void DArray::InsertAt(int nIndex, double dValue)
{
	assert(nIndex >= 0 && nIndex <= m_nSize); // nIndex == m_nSize is legal
	Reserve(m_nSize + 1);

	for (int i = m_nSize; i > nIndex; i--)
		m_pData[i] = m_pData[i - 1];

	m_pData[nIndex] = dValue;
	m_nSize++;
	std::cout << "InsertAt: index = " << nIndex << ", value = " << dValue << std::endl;
}

// overload operator '='
DArray &DArray::operator=(const DArray &arr)
{
	Reserve(arr.m_nSize); // allocate enough memory
	m_nSize = arr.m_nSize;
	memcpy(m_pData, arr.m_pData, m_nSize * sizeof(double));
	return *this;
}
