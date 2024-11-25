#include <iostream>
#include <gtest/gtest.h>
#include "hw1.h"
using namespace algebra;
// 测试矩阵乘法方法的执行时间
void testMatrixMultiplication() {
    size_t n = 512;  // 测试矩阵的维度

    // 生成随机矩阵
    Matrix A = random(n, n, 0.0, 10.0);  // A 是 n x n 的矩阵
    Matrix B = random(n, n, 0.0, 10.0);  // B 是 n x n 的矩阵

    // 记录 multiply_simple 的执行时间
    auto start = std::chrono::high_resolution_clock::now();
    Matrix result_simple = multiply_simple(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_simple = end - start;
    std::cout << "Time taken by simple multiplication: " << duration_simple.count() << " seconds." << std::endl;

    // 记录 multiplyOptimized 的执行时间
    start = std::chrono::high_resolution_clock::now();
    Matrix result_optimized = multiplyOptimized(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_optimized = end - start;
    std::cout << "Time taken by optimized multiplication: " << duration_optimized.count() << " seconds." << std::endl;
    // 检查结果是否一致
    if (result_simple == result_optimized) {
        std::cout << "All multiplication methods produce the same result." << std::endl;
    }
    else {
        std::cout << "The results of different multiplication methods are different." << std::endl;
    }
}
int main(int argc, char **argv)
{
    if (false) // make false to run unit-tests
    {
        testMatrixMultiplication();
        // debug section
    }
    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}