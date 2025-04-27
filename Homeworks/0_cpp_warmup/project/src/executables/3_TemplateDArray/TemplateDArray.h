#pragma once
#include <iostream>
#include <cassert>
#include <cstring>

template <typename T>
class TemplateDArray
{
public:
    TemplateDArray();                          // default constructor
    TemplateDArray(int nSize, T dValue = 0);   // set an array with default values
    TemplateDArray(const TemplateDArray &arr); // copy constructor
    ~TemplateDArray();                         // deconstructor

    void Print() const; // print the elements of the array

    int GetSize() const;     // get the size of the array
    void SetSize(int nSize); // set the size of the array

    const T &GetAt(int nIndex) const; // get an element at an index
    void SetAt(int nIndex, T dValue); // set the value of an element

    T &operator[](int nIndex);             // overload operator '[]'
    const T &operator[](int nIndex) const; // overload operator '[]'

    void PushBack(T dValue);             // add a new element at the end of the array
    void DeleteAt(int nIndex);           // delete an element at some index
    void InsertAt(int nIndex, T dValue); // insert a new element at some index

    TemplateDArray &operator=(const TemplateDArray &arr); // overload operator '='

private:
    void Init();
    void Free();
    void Reserve(int nSize);

private:
    T *m_pData;
    int m_nSize;
    int m_nMax;
};

#include "TemplateDArray.inl"