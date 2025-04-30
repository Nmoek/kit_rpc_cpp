/**
 * @file test_caller.cpp
 * @brief 测试发起RPC调用
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-30 12:45:29
 * @copyright Copyright (c) 2025 Kewin Li
 */

#include "rpc_header.pb.h"
#include "user.pb.h"
#include "kit_rpc_application.h"
#include "rpc_channel.h"
#include "user.pb.h"

#include <fstream>
#include <gtest/gtest.h>
#include "muduo/net/TcpClient.h"
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"

namespace mn = ::muduo::net;
using namespace kit_rpc;

// 全局变量存储命令行参数
static std::vector<char*> sg_args;

// 通过裸连接调用方式模拟RPC调用
TEST(TestCaller, DISABLED_mock_sendReq)
{
    // 参数 args
    kit_rpc::LoginRequest req;
    req.set_name("ljk");
    req.set_pwd("666666");

    /* 以下这部分逻辑可以封装起来后其实就变成 RPC调用参考UserService */
    std::string args_str;
    if(!req.SerializePartialToString(&args_str))
    {
        std::cerr << "req.SerializePartialToString error" << std::endl;
        return;
    }
    uint32_t args_size = (uint32_t)args_str.size();
    // rpc 头
    kit_rpc::RpcHeader header;

    header.set_service_name("UserServiceRpc");
    header.set_method_name("Login");
    header.set_args_size(args_size);

    std::string head_str;
    if(!header.SerializePartialToString(&head_str))
    {
        std::cerr << "req.SerializePartialToString error" << std::endl;
        return;
    }
    uint32_t header_size = (uint32_t)head_str.size();

    std::string send_data;

    uint32_t len = 0;
    send_data.insert(len, (char*)&header_size, sizeof(header_size));
    len += sizeof(header_size);
    send_data.insert(len, head_str.data(), header_size);
    len += header_size;
    send_data.insert(len, args_str.data(), args_size);
    len += args_size;

    std::cout << "send_data size " << send_data.size() << std::endl;

    mn::EventLoop loop;
    mn::InetAddress addr("127.0.0.1", 8000);
    mn::TcpClient client(&loop, addr, "TestSendRpcReq");
    mn::TcpConnectionPtr conn_rpc;

    client.setConnectionCallback([&](const mn::TcpConnectionPtr& conn){
        if(conn->connected())
        {
            std::cout << "connect server OK!" << std::endl;
            conn->send(send_data.data(), len);
        }
        else
        {
            std::cout << "disconnect...!" << std::endl;
            conn->connectDestroyed();
            client.connect();
        }

    });

    client.setWriteCompleteCallback([&](const mn::TcpConnectionPtr& conn){
        std::cout << "send ok" << std::endl;
    });


    // rpc 响应
    client.setMessageCallback([](const mn::TcpConnectionPtr& conn, mn::Buffer* buffer, ::muduo::Timestamp time_stamp){

        auto data = buffer->retrieveAllAsString();
        kit_rpc::LoginResponse resp;
        if(!resp.ParseFromString(data))
        {
            std::cerr << "LoginResponse parse error" << std::endl;
            return;
        }
        kit_rpc::ResultCode result = resp.result();

        std::cout << "===========Response recv============ size: " << data.size() << std::endl;
        std::cout << "errcode: " << result.errcode() << std::endl;
        std::cout << "errmsg: " << result.errmsg() << std::endl;
        std::cout << "success: " << resp.success() << std::endl;
        conn->shutdown();

        sleep(1);
    });

    client.connect();
    std::cout << "client start...." << std::endl;

    loop.loop();
}

// 发起真正RPC调用
TEST(TestCaller, sendReq)
{
    // 框架初始化
    auto& k = KitRpcApplication::GetInstace();

    char *argTmp[sg_args.size()];
    for(int i = 0 ;i < sg_args.size();++i)
        argTmp[i] = sg_args[i];

    k.Init(sg_args.size(), argTmp);

    // 请求参数
    kit_rpc::LoginRequest req;
    req.set_name("zhangsan");
    req.set_pwd("1891891981");
    // 响应数据
    kit_rpc::LoginResponse resp;
    // 回调函数
    gp::Closure* done = nullptr;

    // 同步发起调用
    UserServiceRpc_Stub stub(new KitRpcChannel);
    stub.Login(nullptr, &req, &resp, done);

    EXPECT_EQ(resp.result().errcode(), 0);
    EXPECT_EQ(resp.result().errmsg(), "Login OK");
    EXPECT_EQ(resp.success(), true);

    std::cout << "==========RPC Request OK ===========" << std::endl;
    std::cout << "errcode: " << resp.result().errcode() << std::endl;
    std::cout << "errmsg: " << resp.result().errmsg() << std::endl;
    std::cout << "success: " << resp.success() << std::endl;
}

int main(int argc, char **argv)
{
    // 保存所有参数
    for (int i = 0; i < argc; ++i)
        sg_args.push_back(argv[i]);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}