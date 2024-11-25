#include "hw1.h"
#include <stdexcept> 
#include <random>
#include <iomanip> 
#include <iostream>
namespace algebra{

    Matrix CreatMatrix(int rows, int cols,double value){
        if (rows <= 0 || cols <= 0){
            throw std::invalid_argument("rows and cols must be positive");
        };
        return Matrix(rows, std::vector<double>(cols, value));
    }
    Matrix zeros(size_t n, size_t m){ return CreatMatrix(n, m, 0);}
    Matrix ones(size_t n, size_t m){ return CreatMatrix(n, m, 1);}
    Matrix random(size_t n, size_t m, double min, double max){
        if (n <= 0 || m <= 0){
            throw std::invalid_argument("rows and cols must be positive");
        }
        if (min > max){
            throw std::invalid_argument("min must be less than max");
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        Matrix matrix(n, std::vector<double>(m));
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                double randomValue = std::uniform_real_distribution<double>(min, max)(gen);
                matrix[i][j] = randomValue;
            }
        }
        return matrix;
    }
    void show(const Matrix& matrix){
        std::cout << std::fixed << std::setprecision(3);
        int row=matrix.size();
        int col=matrix[0].size();
        // 打印二维矩阵
        for (const auto& row : matrix) {
            for (const auto& val : row) {
                std::cout << std::setw(2) << val << " ";  // 设置宽度为10，并打印每个元素
            }
            std::cout << std::endl;  // 每行打印完后换行
        }
    }
    Matrix multiply(const Matrix& matrix, double c){
        Matrix result = matrix;
        for (int i = 0; i < result.size(); i++) {
            for (int j = 0; j < result[0].size(); j++) {
                result[i][j] *= c;
            }
        }
        return result;
    }
    Matrix multiply_simple(const Matrix& matrix1, const Matrix& matrix2){
        if (matrix1[0].size() != matrix2.size()) {
            throw std::invalid_argument("The number of columns in matrix1 must be equal to the number of rows in matrix2");
        }
        Matrix result(matrix1.size(), std::vector<double>(matrix2[0].size()));
        for (int i = 0; i < matrix1.size(); i++) {
            for (int j = 0; j < matrix2[0].size(); j++) {
                for (int k = 0; k < matrix1[0].size(); k++) {
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
        return result;
    }
    Matrix multiplyOptimized(const Matrix& A, const Matrix& B){
        //空矩阵
        if (A.empty() || B.empty()) {
            return Matrix();
        }
        int m = A.size();
        int n = A[0].size();
        int p = B[0].size();
        if (A[0].size() != B.size()) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }
        Matrix C(m, std::vector<double>(p, 0));
        // 优化：按行优先访问，减少缓存未命中
        for (int i = 0; i < m; ++i) {
            for (int k = 0; k < n; ++k) {
                for (int j = 0; j < p; ++j) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return C;
    }
    Matrix add(const Matrix& A, const Matrix& B) {
        int n = A.size();
        Matrix C(n, std::vector<double>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                C[i][j] = A[i][j] + B[i][j];
            }
        }
        return C;
    }
    // 矩阵减法
    Matrix subtract(const Matrix& A, const Matrix& B) {
        int n = A.size();
        Matrix C(n, std::vector<double>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                C[i][j] = A[i][j] - B[i][j];
            }
        }
        return C;
    }
    // 矩阵扩展到最近的 2 的幂次方大小
    Matrix padMatrix(const Matrix& A) {
        int n = A.size();
        int m = A[0].size();
        
        // 计算最接近的 2 的幂次方
        int newSize = std::pow(2, std::ceil(std::log2(std::max(n, m))));
        
        // 创建一个新的矩阵，初始化为 0
        Matrix padded(newSize, std::vector<double>(newSize, 0));
        
        // 将原矩阵复制到填充矩阵中
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                padded[i][j] = A[i][j];
            }
        }
        
        return padded;
    }
    Matrix multiply_strassen(const Matrix& A, const Matrix& B) {
        int n = A.size();
        // 基本情况：当矩阵大小为 1x1 时，直接乘
        if (n == 1) {
            Matrix C(1, std::vector<double>(1, 0));
            C[0][0] = A[0][0] * B[0][0];
            return C;
        }

        // 分割矩阵 A 和 B 为 4 个子矩阵
        int mid = n / 2;
        
        Matrix A11(mid, std::vector<double>(mid));
        Matrix A12(mid, std::vector<double>(mid));
        Matrix A21(mid, std::vector<double>(mid));
        Matrix A22(mid, std::vector<double>(mid));
        Matrix B11(mid, std::vector<double>(mid));
        Matrix B12(mid, std::vector<double>(mid));
        Matrix B21(mid, std::vector<double>(mid));
        Matrix B22(mid, std::vector<double>(mid));
        
        // 将矩阵 A 和 B 分割成 4 个子矩阵
        for (int i = 0; i < mid; ++i) {
            for (int j = 0; j < mid; ++j) {
                A11[i][j] = A[i][j];
                A12[i][j] = A[i][j + mid];
                A21[i][j] = A[i + mid][j];
                A22[i][j] = A[i + mid][j + mid];

                B11[i][j] = B[i][j];
                B12[i][j] = B[i][j + mid];
                B21[i][j] = B[i + mid][j];
                B22[i][j] = B[i + mid][j + mid];
            }
        }

        // 计算 7 个 M 矩阵
        Matrix M1 = multiply_strassen(add(A11, A22), add(B11, B22));  // M1 = (A11 + A22) * (B11 + B22)
        Matrix M2 = multiply_strassen(add(A21, A22), B11);  // M2 = (A21 + A22) * B11
        Matrix M3 = multiply_strassen(A11, subtract(B12, B22));  // M3 = A11 * (B12 - B22)
        Matrix M4 = multiply_strassen(A22, subtract(B21, B11));  // M4 = A22 * (B21 - B11)
        Matrix M5 = multiply_strassen(add(A11, A12), B22);  // M5 = (A11 + A12) * B22
        Matrix M6 = multiply_strassen(subtract(A21, A11), add(B11, B12));  // M6 = (A21 - A11) * (B11 + B12)
        Matrix M7 = multiply_strassen(subtract(A12, A22), add(B21, B22));  // M7 = (A12 - A22) * (B21 + B22)

        // 计算最终的 C11, C12, C21, C22
        Matrix C11 = add(subtract(add(M1, M4), M5), M7);
        Matrix C12 = add(M3, M5);
        Matrix C21 = add(M2, M4);
        Matrix C22 = add(subtract(add(M1, M3), M2), M6);

        // 合并结果矩阵 C
        Matrix C(n, std::vector<double>(n));
        for (int i = 0; i < mid; ++i) {
            for (int j = 0; j < mid; ++j) {
                C[i][j] = C11[i][j];
                C[i][j + mid] = C12[i][j];
                C[i + mid][j] = C21[i][j];
                C[i + mid][j + mid] = C22[i][j];
            }
        }
        return C;
    }
    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2){return multiplyOptimized(matrix1, matrix2);}
    Matrix sum(const Matrix& matrix, double c){
        if (matrix.size() == 0){
            return matrix;
        }
        Matrix C(matrix.size(), std::vector<double>(matrix[0].size(),0));
        for(int i = 0; i < matrix.size(); ++i){
            for(int j = 0; j < matrix[0].size(); ++j){
                C[i][j] = matrix[i][j] + c;
            }
        }
        return C;
    }
    Matrix sum(const Matrix& A, const Matrix& B){
        //判断是否为空
        if(A.size() == 0){
            return B;
        }
        if(A.size() != B.size() || A[0].size() != B[0].size()){
            throw std::logic_error("Matrix dimensions do not match");
        }
        Matrix C(A.size(), std::vector<double>(A[0].size(),0));
        for(int i = 0; i < A.size(); ++i){
            for(int j = 0; j < A[0].size(); ++j){
                C[i][j] = A[i][j] + B[i][j];
            }
        }
        return C;
    }
    Matrix transpose(const Matrix& matrix){
        if (matrix.size() == 0) {
            return matrix;
        }
        Matrix C= zeros(matrix[0].size(), matrix.size());
        for(int i = 0; i < matrix.size(); ++i){
            for(int j = 0; j < matrix[0].size(); ++j){
                C[j][i] = matrix[i][j];
            }
        }
        return C;
    }
    Matrix minor(const Matrix& matrix, size_t n, size_t m){
        if (matrix.size() == 0) {
            return matrix;
        }
        Matrix C= zeros(matrix.size()-1, matrix[0].size()-1);
        for(int i=0;i<C.size();i++){
            for(int j=0;j<C[0].size();j++){
                if(i<n && j<m){
                    C[i][j] = matrix[i][j];
                }
                else if(i<n && j>=m){
                    C[i][j] = matrix[i][j+1];
                }
                else if(i>=n && j<m){
                    C[i][j] = matrix[i+1][j];
                }
                else{
                    C[i][j] = matrix[i+1][j+1];
                }
            }
        }
        return C;
    }
    double determinant(const Matrix& matrix){
        if (matrix.size() == 0) {
            return 1;
        }
        if (matrix.size() == 1) {
            return matrix[0][0];
        }
        //矩阵不和法
        if (matrix.size() != matrix[0].size()) {
            throw std::logic_error("Matrix is not square");
        }
        double det = 0;
        Matrix minorMatrix=matrix; // 创建一个临时矩阵，用于计算每个子矩阵的行列式
        for (int i = 0; i < matrix.size(); ++i) {
            minorMatrix = minor(matrix, 0, i); // 计算第 i 列的子矩阵
            det += pow(-1, i) * matrix[0][i] * determinant(minorMatrix); // 递归计算子矩阵的行列式
        }
        return det;
    }
    Matrix inverse(const Matrix& matrix){
        if (matrix.size() == 0) {
            return matrix;
        }
        if (matrix.size() != matrix[0].size()) {
            throw std::logic_error("Matrix is not square");
        }
        if (determinant(matrix) == 0) {
            throw std::logic_error("Matrix is not invertible");
        }
        Matrix C= zeros(matrix.size(), matrix[0].size());
        double determinant_matrix =determinant(matrix);
        std::cout<<determinant_matrix<<std::endl;
        for(int i=0;i<C.size();i++){
            for(int j=0;j<C[0].size();j++){
                C[j][i] = pow(-1,i+j) * determinant(minor(matrix, i, j))/ determinant_matrix;;
            }
        }
        return C;
    }
    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis){
        if (matrix1.size() == 0) {
            return matrix2;
        }
        if (matrix2.size() == 0) {
            return matrix1;
        }
        if (axis == 0) {
            if (matrix1[0].size() != matrix2[0].size()) {
                throw std::logic_error("Matrix dimensions do not match");
            }
            Matrix C= zeros(matrix1.size() + matrix2.size(), matrix1[0].size());
            for(int i = 0; i < matrix1.size(); ++i){
                for(int j = 0; j < matrix1[0].size(); ++j){
                    C[i][j] = matrix1[i][j];
                }
            }
            for(int i = 0; i < matrix2.size(); ++i){
                for(int j = 0; j < matrix2[0].size(); ++j){
                    C[i+matrix1.size()][j] = matrix2[i][j];
                }
            }
            return C;
        }else{
            if (matrix1.size() != matrix2.size()) {
                throw std::logic_error("Matrix dimensions do not match");
            }
            Matrix C= zeros(matrix1.size(), matrix1[0].size() + matrix2[0].size());
            for(int i = 0; i < matrix1.size(); ++i){
                for(int j = 0; j < matrix1[0].size(); ++j){
                    C[i][j] = matrix1[i][j];
                }
            }
            for(int i = 0; i < matrix2.size(); ++i){
                for(int j = 0; j < matrix2[0].size(); ++j){
                    C[i][j+matrix1[0].size()] = matrix2[i][j];
                }
            }
            return C;
        }
    }
    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2){
        //check if the r1 and r2 are valid
        if(r1>=matrix.size()||r2>=matrix.size()){
            throw std::logic_error("Invalid row index");
        }
        if(r1==r2){
            return matrix;
        }
        if (matrix.size() == 0) {
            return matrix;
        }
        Matrix C= zeros(matrix.size(), matrix[0].size());
        for(int i = 0; i < matrix.size(); ++i){
            for(int j = 0; j < matrix[0].size(); ++j){
                if(i==r1){
                    C[i][j] = matrix[r2][j];
                }else if(i==r2){
                    C[i][j] = matrix[r1][j];
                }else{
                    C[i][j] = matrix[i][j];
                }
            }
        }
        return C;
    }
    Matrix ero_multiply(const Matrix& matrix, size_t r, double k){
        if(r>=matrix.size()){
            throw std::logic_error("Invalid row index");
        }

        if (matrix.size() == 0) {
            return matrix;
        }
        Matrix C= zeros(matrix.size(), matrix[0].size());
        for(int i = 0; i < matrix.size(); ++i){
            for(int j = 0; j < matrix[0].size(); ++j){
                if(i==r){
                    C[i][j] = matrix[i][j]*k;
                }else{
                    C[i][j] = matrix[i][j];
                }
            }
        }
        return C;
    }
    Matrix ero_sum(const Matrix& matrix, size_t r1, double k, size_t r2){
        if(r1>=matrix.size()||r2>=matrix.size()){
            throw std::logic_error("Invalid row index");
        }
        if (matrix.size() == 0) {
            return matrix;
        }
        Matrix C= zeros(matrix.size(), matrix[0].size());
        for(int i = 0; i < matrix.size(); ++i){
            for(int j = 0; j < matrix[0].size(); ++j){
                if(i==r2){
                    C[i][j] = matrix[i][j] + matrix[r1][j]*k;
                }else{
                    C[i][j] = matrix[i][j];
                }
            }
        }
        return C;
    }
    Matrix upper_triangular(const Matrix& matrix){
        if (matrix.size() == 0) {
            return matrix;
        }
        Matrix C=matrix;

        int i=0,j=0; //i is the row index, j is the column index
        while(i<C.size()&&j<C[0].size()){
            int index=i; 
            while(index<C.size()){
                if(C[index][j]!=0){
                    C=ero_swap(C,i,index);
                    break;
                }
                index++;
            }
            if(index<C.size()){
                for(int k=i+1;k<C.size();k++){
                    C=ero_sum(C,i,-C[k][j]/C[i][j],k);
                }
            }
            i++;
            j++;
        }
        if(i<C.size()||j<C[0].size()){
            throw std::logic_error("Matrix is not upper triangular");
        }
        show(C);
        return C;

    }

}
