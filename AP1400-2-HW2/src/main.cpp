
#include <iostream>
#include <gtest/gtest.h>
#include "client.h"
#include "server.h"


int main(int argc, char **argv)
{
    if (false) // make false to run unit-tests
    {
        Server server{};
        pending_trxs.clear();
        auto bryan{server.add_client("bryan")};
        auto clint{server.add_client("clint")};
        auto sarah{server.add_client("sarah")};
        bryan->transfer_money("clint", 1);
        clint->transfer_money("sarah", 2.5);
        sarah->transfer_money("bryan", 0.5);
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
