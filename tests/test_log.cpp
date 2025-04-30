/**
 * @file test_log.cpp
 * @brief 日志测试
 * @author Kewin Li
 * @version 1.0
 * @date 2025-05-01 01:05:49
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include <gtest/gtest.h>
#include "logger.h"



TEST(TestLogger, log)
{
    RPC_INFO("test %d \n", 1);
    RPC_INFO("test %d \n", 2);
    RPC_ERR("test %d \n", 3);
    RPC_ERR("test %d \n", 4);

}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}