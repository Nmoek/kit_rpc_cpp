/**
 * @file test_callee.cpp
 * @brief  测试服务器被远程调用
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-26 23:01:32
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include <gtest/gtest.h>

#include "userservice.h"
#include "kit_rpc_application.h"
#include "rpc_provider.h"

using namespace kit_rpc;

// 全局变量存储命令行参数
static std::vector<char*> sg_args;

class MyRun: public ::google::protobuf::Closure
{
public:
    MyRun() = default;
    ~MyRun() = default;
    void Run()
    {
        std::cout << "MyRun::run()" << std::endl;
    }
};

TEST(TestCallee, mock_UserService)
{
    UserService s;
    LoginRequest req;
    req.set_name("ljk");
    req.set_pwd("123456");

    LoginResponse resp;
    ::google::protobuf::Closure* c = new MyRun;
    s.Login(nullptr, &req, &resp, c);
    delete c;
}


TEST(TestCallee, rpc_init)
{
    auto& k = KitRpcApplication::GetInstace();

    char *argTmp[sg_args.size()];
    for(int i = 0 ;i < sg_args.size();++i)
        argTmp[i] = sg_args[i];

    k.Init(sg_args.size(), argTmp);

    std::string ip = k.getConfig()->get("rpc_server_ip");
    uint16_t port = k.getConfig()->get<int>("rpc_server_port");
    Provide p(ip, port, "UserService");

    p.notifyService(std::shared_ptr<UserService>());
    p.run();

}

int main(int argc, char **argv)
{
    // 保存所有参数
    for (int i = 0; i < argc; ++i)
        sg_args.push_back(argv[i]);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
