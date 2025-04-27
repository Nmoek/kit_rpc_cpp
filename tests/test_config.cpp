/**
 * @file test_config.cpp
 * @brief 测试RPC配置
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-28 02:24:20
 * @copyright Copyright (c) 2025 Kewin Li
 */

#include <gtest/gtest.h>
#include "rpc_config.h"

using namespace kit_rpc;

TEST(TestConfig, load_file)
{
    RpcConfig c("../bin/test.ini", RpcConfig::INI_FILE);
    c.loadFile();
    c.loadConfigFromFile();

    c.setFile("../bin/test.ini", RpcConfig::JSON_FILE);
    c.loadFile();
    c.loadConfigFromFile();

    c.setFile("../bin/test.ini", RpcConfig::YAML_FILE);
    c.loadFile();
    c.loadConfigFromFile();
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}