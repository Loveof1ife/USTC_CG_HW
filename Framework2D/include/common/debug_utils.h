#pragma once
#include <Eigen/Sparse>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#define NOMINMAX

namespace DebugUtils
{
class Debugger
{
   public:
    static int level;

    static void parse_command_line(int argc, char* argv[])
    {
        level = 0;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "-d" || arg == "--debug")
            {
                if (i + 1 < argc)
                    level = std::atoi(argv[i + 1]);
            }
        }
    }

    template<typename... Args>
    static void log(int required_level, Args&&... args)
    {
        if (level >= required_level)
        {
            (std::cout << ... << args) << '\n';
        }
    }
};

inline void AbortWithErrorCode(const std::string& message, int error_code);

template<typename Value>
void Assert(
    const char* expression,
    const Value value,
    const char* file_name,
    unsigned line_num)
{
    if (value)
        return;
    std::ostringstream oss;
    oss << file_name << ":" << line_num << ": Assert(" << expression
        << ") failed";
    AbortWithErrorCode(oss.str(), 1);
}

template<typename ValueFirst, typename ValueSecond>
void AssertEqual(
    const char* expression_first,
    const ValueFirst value_first,
    const char* expression_second,
    const ValueSecond value_second,
    const char* file_name,
    unsigned line_num)
{
    if (value_first == value_second)
        return;
    std::ostringstream oss;
    oss << file_name << ":" << line_num << ": AssertEqual(" << expression_first
        << ", " << expression_second << ") failed (" << value_first
        << " != " << value_second << ")";
    AbortWithErrorCode(oss.str(), 1);
}

inline void Abort(const std::string& message)
{
    AbortWithErrorCode(message, 1);
}

inline void AbortWithErrorCode(const std::string& message, int error_code)
{
    std::fprintf(stderr, "Error: %s\n", message.c_str());
    std::exit(error_code);
}

inline void debugSparseMatrix(
    const Eigen::SparseMatrix<double>& mat,
    int max_rows = 5)
{
    std::cout << "Matrix size: " << mat.rows() << "x" << mat.cols()
              << " (non-zeros: " << mat.nonZeros() << ")\n";
    for (int k = 0; k < mat.outerSize() && k < max_rows; ++k)
    {
        std::cout << "Row " << k << ": ";
        for (Eigen::SparseMatrix<double>::InnerIterator it(mat, k); it; ++it)
        {
            std::cout << "(" << it.col() << ":" << it.value() << ") ";
        }
        std::cout << "\n";
    }
}

inline void checkDiagonalDominance(const Eigen::SparseMatrix<double>& mat)
{
    for (int k = 0; k < mat.outerSize(); ++k)
    {
        double diag = 0;
        double row_sum = 0;
        bool has_diag = false;

        for (Eigen::SparseMatrix<double>::InnerIterator it(mat, k); it; ++it)
        {
            if (it.col() == k)
            {
                diag = std::abs(it.value());
                has_diag = true;
            }
            else
            {
                row_sum += std::abs(it.value());
            }
        }

        if (!has_diag)
        {
            std::cerr << "WARNING: Row " << k << " has no diagonal element\n";
        }
        else if (diag <= row_sum)
        {
            std::cerr << "WARNING: Row " << k << " is not diagonally dominant ("
                      << diag << " <= " << row_sum << ")\n";
        }
    }
}

inline void checkSymmetry(const Eigen::SparseMatrix<double>& mat)
{
    Eigen::SparseMatrix<double> matT = mat.transpose();
    double symmtry_error = (mat - matT).norm();

    std::cout << "Matrix symmetry error: " << symmtry_error << std::endl;
}
}  // namespace DebugUtils