#ifndef AP_HW1_H
#define AP_HW1_H
#include<vector>
using std::size_t;
namespace algebra{
using Matrix = std::vector<std::vector<double>>;
    Matrix zeros(size_t n, size_t m);
    Matrix ones(size_t n, size_t m);
    Matrix random(size_t n, size_t m, double min, double max);
    void show(const Matrix& matrix);
    Matrix multiply(const Matrix& matrix, double c);
    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2);
    Matrix multiplyOptimized(const Matrix& A, const Matrix& B);
    Matrix multiply_simple(const Matrix& matrix1, const Matrix& matrix2);
    Matrix multiply_strassen(const Matrix& matrix1, const Matrix& matrix2);
    Matrix subtract(const Matrix& A, const Matrix& B);
    Matrix sum(const Matrix& A, const Matrix& B);
    Matrix sum(const Matrix& matrix, double c);
    Matrix transpose(const Matrix& matrix);
    Matrix minor(const Matrix& matrix, size_t n, size_t m);
    double determinant(const Matrix& matrix);
    // implement this function so that it generates the matrix's inverse.
    Matrix inverse(const Matrix& matrix);
    //implement this function so that it will concatenate matrix1 and matrix2 along the specified axis. (axis=0: on top of each other | axis=1: alongside each other).
    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0);
    //swapping two rows.
    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2);
    //multiplying a row into a constant number.
    Matrix ero_multiply(const Matrix& matrix, size_t r, double c);
    //multiplying a row into a constant number and add it to another row.
    Matrix ero_sum(const Matrix& matrix, size_t r1, double k, size_t r2);
    Matrix upper_triangular(const Matrix& matrix);
}

#endif //AP_HW1_H
