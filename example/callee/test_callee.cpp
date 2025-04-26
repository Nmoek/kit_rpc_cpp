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

    k.Init(0, nullptr);

    Provide p;
    p.notifyService(new UserService);
    p.run();

}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
