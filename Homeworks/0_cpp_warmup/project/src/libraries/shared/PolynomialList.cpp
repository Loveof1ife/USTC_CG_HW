#include "PolynomialList.h"

PolynomialList::PolynomialList(const PolynomialList &other)
{
    if (other.m_Polynomial.empty())
        return;
    for (const auto &term : other.m_Polynomial)
    {
        Term newTerm(term.deg, term.cof);
        m_Polynomial.emplace_back(newTerm);
    }
    compress();
}

PolynomialList::PolynomialList(const std::string &file)
{
    if (!ReadFromFile(file))
    {
        std::cerr << "Error: Cannot read from file " << file << std::endl;
        return;
    }
    compress();
}

PolynomialList::PolynomialList(const double *cof, const int *deg, int n)
{
    for (int i = 0; i < n; ++i)
    {
        Term newTerm(deg[i], cof[i]);
        m_Polynomial.push_back(newTerm);
    }
    compress();
}

PolynomialList::PolynomialList(const std::vector<int> &deg, const std::vector<double> &cof)
{
    assert(deg.size() == cof.size());
    for (size_t i = 0; i < deg.size(); ++i)
    {
        Term newTerm(deg[i], cof[i]);
        m_Polynomial.push_back(newTerm);
    }
    compress();
}

double PolynomialList::coff(int i) const
{
    for (const auto &term : m_Polynomial)
    {
        if (term.deg == i)
            return term.cof;
    }
    return 0.;
}

double &PolynomialList::coff(int i)
{
    for (auto &term : m_Polynomial)
    {
        if (term.deg == i)
            return term.cof;
    }

    m_Polynomial.emplace_back(i, 0.0);
    return m_Polynomial.back().cof; //
}

void PolynomialList::compress()
{
    if (m_Polynomial.empty())
        return;

    m_Polynomial.sort([](const Term &a, const Term &b)
                      { return a.deg > b.deg; });

    auto itr = m_Polynomial.begin();
    while (itr != m_Polynomial.end())
    {
        auto next_iter = std::next(itr);
        bool erased = false;
        if (fabs(itr->cof < EPSILON))
        {
            itr = m_Polynomial.erase(itr);
        }
        else if (next_iter != m_Polynomial.end() && itr->deg == next_iter->deg)
        {
            next_iter->cof += itr->cof;
            itr = m_Polynomial.erase(itr);
            erased = true;
        }
        if (!erased)
        {
            ++itr;
        }
    }
}

PolynomialList PolynomialList::operator+(const PolynomialList &right) const
{
    std::map<int, double, std::greater<int>> degreeMap;
    for (const auto &term : m_Polynomial)
    {
        degreeMap[term.deg] += term.cof;
    }
    for (const auto &term : right.m_Polynomial)
    {
        degreeMap[term.deg] += term.cof;
    }
    PolynomialList addResult;
    for (const auto &pair : degreeMap)
    {
        if (pair.second != 0.0)
        {
            addResult.AddOneTerm(Term(pair.first, pair.second));
        }
    }
    return addResult;
}

PolynomialList PolynomialList::operator-(const PolynomialList &right) const
{
    std::map<int, double, std::greater<int>> degreeMap;
    for (const auto &term : m_Polynomial)
    {
        degreeMap[term.deg] += term.cof;
    }
    for (const auto &term : right.m_Polynomial)
    {
        degreeMap[term.deg] -= term.cof;
    }
    PolynomialList subResult;
    for (const auto &pair : degreeMap)
    {
        if (pair.second != 0.0)
        {
            subResult.AddOneTerm(Term(pair.first, pair.second));
        }
    }
    return subResult;
}

PolynomialList PolynomialList::operator*(const PolynomialList &right) const
{
    std::map<int, double, std::greater<int>> degreeMap;
    for (const auto &term1 : m_Polynomial)
    {
        for (const auto &term2 : right.m_Polynomial)
        {
            degreeMap[term1.deg + term2.deg] += term1.cof * term2.cof;
        }
    }
    PolynomialList mulResult;
    for (const auto &pair : degreeMap)
    {
        if (pair.second != 0.0)
        {
            mulResult.AddOneTerm(Term(pair.first, pair.second));
        }
    }
    return mulResult;
}

PolynomialList &PolynomialList::operator=(const PolynomialList &right)
{
    if (this == &right)
        return *this;
    m_Polynomial.clear();

    for (const auto &term : right.m_Polynomial)
    {
        m_Polynomial.emplace_back(term.deg, term.cof);
    }
    compress();
    return *this;
}

void PolynomialList::Print() const
{
    for (auto &term : m_Polynomial)
    {
        if (&term != &m_Polynomial.front())
        {
            std::cout << " ";
            if (term.cof > 0)
                std::cout << "+";
        }
        std::cout << term.cof;
        if (term.deg > 0)
            std::cout << "x^" << term.deg;
    }
    std::cout << std::endl;
}

bool PolynomialList::ReadFromFile(const std::string &file)
{
    m_Polynomial.clear();

    std::ifstream infile(file);
    if (!infile.is_open())
    {
        std::cerr << "Error: Cannot open file " << file << std::endl;
        return false;
    }

    // Read and verify header
    char ch;
    int termCount;
    if (!(infile >> ch >> termCount) || ch != 'P')
    {
        std::cerr << "Error: Invalid file format" << std::endl;
        infile.close();
        return false;
    }

    // Read terms
    int degree;
    double coefficient;
    int termsRead = 0;
    while (termsRead < termCount && infile >> degree >> coefficient)
    {
        AddOneTerm(Term(degree, coefficient));
        termsRead++;
    }

    // Verify we read all expected terms
    if (termsRead != termCount)
    {
        std::cerr << "Error: Expected " << termCount << " terms but found " << termsRead << std::endl;
        infile.close();
        return false;
    }

    infile.close();

    compress();
    return true;
}
PolynomialList::Term &PolynomialList::AddOneTerm(const Term &term)
{
    m_Polynomial.emplace_back(term.deg, term.cof);
    return m_Polynomial.back();
}
