/**
 * @file rpc_channel.cpp
 * @brief RPC发起调用
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-30 20:06:09
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include "rpc_channel.h"
#include "rpc_header.pb.h"
#include "muduo/net/TcpClient.h"
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "kit_rpc_application.h"
#include "zookeeper_cli.h"

#include <google/protobuf/descriptor.h>
#include <iostream>


namespace mn = ::muduo::net;

namespace kit_rpc {

void KitRpcChannel::CallMethod(
        const gp::MethodDescriptor* method,
        gp::RpcController* controller,
        const gp::Message* request,
        gp::Message* response,
        gp::Closure* done)
{
    const gp::ServiceDescriptor *sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();
    std::string args_str;

    if(!request->SerializePartialToString(&args_str))
    {
        controller->SetFailed("request->SerializePartialToString error");
        return;
    }
    uint32_t args_size = (uint32_t)args_str.size();

    // rpc 头
    RpcHeader header;
    header.set_service_name(service_name);
    header.set_method_name(method_name);
    header.set_args_size(args_size);

    std::string head_str;
    if(!header.SerializePartialToString(&head_str))
    {
        controller->SetFailed("req.SerializePartialToString error");
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

    // 调客户端开始发数据
    // auto& k = KitRpcApplication:: GetInstace();
    // std::string target_server_ip = k.getConfig()->get("rpc_server_ip");
    // uint16_t target_server_port = k.getConfig()->get<int>("rpc_server_port");

    //向Zookeeper查询服务在哪
    ZkClient cli;
    cli.start();
    std::string path = "/";
    path += service_name;
    path += "/";
    path += method_name;

    std::string host = cli.getNodeData(path);
    if(host.empty())
    {
        controller->SetFailed("zookeeper get ip/port is null");
        return;
    }
    auto pos = host.find(":");
    if(-1 == pos)
    {
        controller->SetFailed("zookeeper get ip/port is invalid");
        return;
    }
    std::string target_server_ip = host.substr(0, pos);
    uint16_t target_server_port = std::stoi(host.substr(pos + 1));

    mn::EventLoop loop;
    mn::InetAddress addr(target_server_ip, target_server_port);
    mn::TcpClient client(&loop, addr, method->full_name());

    client.setConnectionCallback([&](const mn::TcpConnectionPtr& conn){
        if(conn->connected())
        {
            conn->send(send_data.data(), len);
        }
        else
        {
            conn->connectDestroyed();
            loop.quit();
        }
    });

    // rpc 响应
    client.setMessageCallback([response, controller](const mn::TcpConnectionPtr& conn, mn::Buffer* buffer, ::muduo::Timestamp time_stamp){

        if(!response->ParseFromString(buffer->retrieveAllAsString().data()))
        {
            controller->SetFailed("response->ParseFromString error");
        }
        conn->shutdown();
    });

    client.connect();
    loop.loop();
}



}   //kit_rpc