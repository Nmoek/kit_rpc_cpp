/**
 * @file test_zoo.cpp
 * @brief  zookeeper客户端测试
 * @author Kewin Li
 * @version 1.0
 * @date 2025-05-01 21:52:02
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include <gtest/gtest.h>

#include "kit_rpc_application.h"
#include "zookeeper_cli.h"

using namespace kit_rpc;

// 全局变量存储命令行参数
static std::vector<char*> sg_args;

TEST(TestZoo, test)
{
    auto &k = KitRpcApplication::GetInstace();

    char *argTmp[sg_args.size()];
    for(int i = 0 ;i < sg_args.size();++i)
        argTmp[i] = sg_args[i];

    k.Init(sg_args.size(), argTmp);

    ZkClient cli;
    cli.start();
    std::string path = "/TestServiceRpc";
    std::string data = "test data";
    cli.createNode(path, data, 0);
    auto data2 = cli.getNodeData(path);
    std::cout << "data= " << data2 << std::endl;

    cli.deleteNode(path);
}

int main(int argc, char **argv)
{
    // 保存所有参数
    for (int i = 0; i < argc; ++i)
        sg_args.push_back(argv[i]);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
