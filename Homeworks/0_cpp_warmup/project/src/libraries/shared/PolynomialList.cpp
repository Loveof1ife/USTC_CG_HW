#include "PolynomialList.h"
#include <algorithm>
PolynomialList::PolynomialList(const PolynomialList &other)
    : m_Polynomial(other.m_Polynomial.begin(), other.m_Polynomial.end())
{
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
    : m_Polynomial(deg.size())
{
    if (deg.size() != cof.size())
    {
        throw std::invalid_argument("Degree and coefficient vectors must have same size");
    }
    std::transform(deg.begin(), deg.end(), cof.begin(), std::back_inserter(m_Polynomial),
                   [](int d, double c)
                   { return Term(d, c); });

    compress();
}

double PolynomialList::coff(int i) const
{
    const auto itr = std::find_if(m_Polynomial.cbegin(), m_Polynomial.cend(),
                                  [i](const Term &term)
                                  { return term.deg == i; });
    return itr != m_Polynomial.cend() ? itr->cof : 0.0;
}

double &PolynomialList::coff(int i)
{
    auto itr = std::find_if(m_Polynomial.begin(), m_Polynomial.end(),
                            [i](const Term &term)
                            { return term.deg == i; });
    if (itr == m_Polynomial.end())
    {
        m_Polynomial.emplace_back(i, 0.0);
        return m_Polynomial.back().cof;
    }
    else
        return itr->cof;
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
        if (fabs(itr->cof < EPSILON))
        {
            itr = m_Polynomial.erase(itr);
            continue;
        }

        auto next_iter = std::next(itr);
        if (next_iter != m_Polynomial.end() && itr->deg == next_iter->deg)
        {
            next_iter->cof += itr->cof;
            itr = m_Polynomial.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
}

PolynomialList PolynomialList::operator+(const PolynomialList &right) const
{
    PolynomialList result;
    auto itr1 = m_Polynomial.begin();
    auto itr2 = right.m_Polynomial.begin();

    while (itr1 != m_Polynomial.end() && itr2 != right.m_Polynomial.end())
    {
        if (itr1->deg > itr2->deg)
        {
            result.m_Polynomial.emplace_back(*itr1);
            ++itr1;
        }
        else if (itr1->deg < itr2->deg)
        {
            result.m_Polynomial.emplace_back(*itr2);
            ++itr2;
        }
        else
        {
            double sum = itr1->cof + itr2->cof;
            if (fabs(sum) > EPSILON)
            {
                result.m_Polynomial.emplace_back(itr1->deg, sum);
            }
            ++itr1;
            ++itr2;
        }
    }

    result.m_Polynomial.insert(result.m_Polynomial.end(), itr1, m_Polynomial.end());
    result.m_Polynomial.insert(result.m_Polynomial.end(), itr2, right.m_Polynomial.end());
    result.compress();
    return result;
}

PolynomialList PolynomialList::operator-(const PolynomialList &right) const
{
    PolynomialList negatedRight(right);
    for (auto &term : negatedRight.m_Polynomial)
    {
        term.cof = -term.cof;
    }
    return *this + negatedRight;
}

PolynomialList PolynomialList::operator*(const PolynomialList &right) const
{
    if (m_Polynomial.empty() || right.m_Polynomial.empty())
        return PolynomialList();
    std::map<int, double, std::greater<int>> degreeMap;
    for (const auto &term1 : m_Polynomial)
    {
        for (const auto &term2 : right.m_Polynomial)
        {
            degreeMap[term1.deg + term2.deg] += term1.cof * term2.cof;
        }
    }
    PolynomialList result;
    for (const auto &[deg, cof] : degreeMap)
    {
        if (cof != 0.0)
        {
            result.AddOneTerm(Term(deg, cof));
        }
    }
    return result;
}

PolynomialList &PolynomialList::operator=(const PolynomialList &right)
{
    if (this == &right)
    {
        return *this;
    }
    m_Polynomial.clear();
    for (const auto &term : right.m_Polynomial)
    {
        m_Polynomial.emplace_back(term.deg, term.cof);
    }
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
    auto itr = m_Polynomial.begin();
    for (; itr != m_Polynomial.end(); ++itr)
    {
        if (itr->deg == term.deg)
        {
            itr->cof += term.cof;
            if (fabs(itr->cof) < EPSILON)
            {
                m_Polynomial.erase(itr);
            }
            return *itr;
        }
        else if (itr->deg < term.deg)
        {
            break;
        }
    }
    return *m_Polynomial.insert(itr, term);
}
