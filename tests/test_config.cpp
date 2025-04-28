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
    RpcConfig c("../default_conf.ini", RpcConfig::INI_FILE);
    c.loadFile();
    c.loadConfigFromFile();

    auto val1 = c.get("rpc_server_ip");
    std::cout << "rpc_server_ip" << ":" << val1 << std::endl;

    auto val2 = c.get<int32_t>("rpc_server_port");
    std::cout << "rpc_server_port" << ":" << val2 << std::endl;

    auto val3 = c.get("zookeeper_ip");
    std::cout << "zookeeper_ip" << ":" << val3 << std::endl;

    auto val4  = c.get<int32_t>("zookeepr_port");
    std::cout << "rpc_server_port" << ":" << val4 << std::endl;

    auto val5  = c.get<int32_t>("aaa");
    std::cout << "aaa" << ":" << val5 << std::endl;

    auto val6  = c.get<float>("bbb");
    std::cout << "bbb" << ":" << val6 << std::endl;
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}