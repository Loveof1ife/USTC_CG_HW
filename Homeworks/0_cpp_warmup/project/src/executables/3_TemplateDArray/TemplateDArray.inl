template <typename T>
void TemplateDArray<T>::Init()
{
    m_pData = nullptr;
    m_nSize = 0;
    m_nMax = 0;
}

template <typename T>
TemplateDArray<T>::TemplateDArray()
{
    Init();
}

template <typename T>
TemplateDArray<T>::TemplateDArray(int nSize, T dValue)
    : m_nSize(nSize), m_nMax(nSize * 2), m_pData(new T[m_nMax])
{
    for (int i = 0; i < nSize; i++)
        m_pData[i] = dValue;
}

// copy constructor
template <typename T>
TemplateDArray<T>::TemplateDArray(const TemplateDArray &arr)
    : m_pData(new T[arr.m_nMax]), m_nSize(arr.m_nSize), m_nMax(arr.m_nMax)
{
    for (int i = 0; i < m_nSize; i++)
        m_pData[i] = arr.m_pData[i];
}

template <typename T>
TemplateDArray<T>::~TemplateDArray()
{
    Free();
}

template <typename T>
void TemplateDArray<T>::Free()
{
    if (m_pData != nullptr)
    {
        delete[] m_pData;
        m_pData = nullptr;
    }
    m_nSize = 0;
    m_nMax = 0;
}

template <typename T>
int TemplateDArray<T>::GetSize() const
{
    return m_nSize;
}

template <typename T>
void TemplateDArray<T>::SetSize(int nSize)
{
    if (nSize == m_nSize)
        return;
    Reserve(nSize);
    for (int i = m_nSize; i < nSize; i++)
        m_pData[i] = 0.;
    m_nSize = nSize;
}

template <typename T>
void TemplateDArray<T>::Reserve(int nSize)
{
    if (nSize < m_nMax)
        return;
    while (nSize > m_nMax)
        m_nMax = m_nMax == 0 ? 1 : m_nMax * 2;

    T *pTemp = new T[m_nMax];

    memcpy(pTemp, m_pData, m_nSize * sizeof(T));
    delete[] m_pData;
    m_pData = pTemp;
}

template <typename T>
const T &TemplateDArray<T>::GetAt(int nIndex) const
{
    assert(nIndex >= 0 && nIndex < m_nSize);
    return m_pData[nIndex];
}

template <typename T>
void TemplateDArray<T>::Print() const
{
    for (int i = 0; i < m_nSize; i++)
        std::cout << m_pData[i] << " ";
    std::cout << std::endl;
}

template <typename T>
void TemplateDArray<T>::SetAt(int nIndex, T dValue)
{
    assert(nIndex >= 0 && nIndex < m_nSize);
    m_pData[nIndex] = dValue;
}

template <typename T>
T &TemplateDArray<T>::operator[](int nIndex)
{
    assert(nIndex >= 0 && nIndex < m_nSize);
    return m_pData[nIndex]; // you should return a correct value
}

template <typename T>
const T &TemplateDArray<T>::operator[](int nIndex) const
{
    assert(nIndex >= 0 && nIndex < m_nSize);
    return m_pData[nIndex]; // you should return a correct value
}

template <typename T>
void TemplateDArray<T>::PushBack(T dValue)
{
    Reserve(m_nSize + 1);
    m_pData[m_nSize] = dValue;
    m_nSize++;
}

template <typename T>
void TemplateDArray<T>::DeleteAt(int nIndex)
{
    assert(nIndex >= 0 && nIndex < m_nSize);
    for (int i = nIndex + 1; i < m_nSize; i++)
        m_pData[i - 1] = m_pData[i];
    m_nSize--;
}

template <typename T>
void TemplateDArray<T>::InsertAt(int nIndex, T dValue)
{
    assert(nIndex >= 0 && nIndex <= m_nSize);
    Reserve(m_nSize + 1);
    for (int i = m_nSize; i > nIndex; i--)
        m_pData[i] = m_pData[i - 1];
    m_pData[nIndex] = dValue;
    m_nSize++;
    std::cout << "InsertAt: index = " << nIndex << ", value = " << dValue << std::endl;
}

// overload operator '='
template <typename T>
TemplateDArray<T> &TemplateDArray<T>::operator=(const TemplateDArray &arr)
{
    Reserve(arr.m_nMax);
    m_nSize = arr.m_nSize;
    memcpy(m_pData, arr.m_pData, m_nSize * sizeof(T));
    return *this;
}