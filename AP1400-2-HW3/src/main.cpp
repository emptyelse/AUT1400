#include <iostream>
#include <gtest/gtest.h>
// #include "bst.h"
// 这是类的定义部分


int main(int argc, char **argv)
{
    if (false) // make false to run unit-tests
    {
        // char *s=malloc(100*sizeof(char));
        // scanf("%s", s);

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