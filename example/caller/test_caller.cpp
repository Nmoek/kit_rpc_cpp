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

#include <fstream>
#include <gtest/gtest.h>
#include "muduo/net/TcpClient.h"
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"

namespace mn = ::muduo::net;

TEST(TestCaller, sendReq)
{
    // 参数 args
    kit_rpc::LoginRequest req;
    req.set_name("ljk");
    req.set_pwd("666666");
    std::string args_str;
    if(!req.SerializePartialToString(&args_str))
    {
        std::cerr << "req.SerializePartialToString error" << std::endl;
        return;
    }

    // rpc 头
    kit_rpc::RpcHeader header;

    header.set_service_name("UserServiceRpc");
    header.set_method_name("Login");
    header.set_args_size(args_str.size());

    std::string head_str;
    if(!header.SerializePartialToString(&head_str))
    {
        std::cerr << "req.SerializePartialToString error" << std::endl;
        return;
    }
    uint32_t header_size = (uint32_t)head_str.size();

    std::string send_data(sizeof(header_size) + header_size + args_str.size(), 0);

    uint32_t len = 0;
    send_data.insert(len, (char*)&header_size);
    len += sizeof(header_size);
    send_data.insert(len, head_str);
    len += header_size;
    send_data.insert(len, args_str);
    len += args_str.size();

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

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}