/**
 * @file rpc_provider.cpp
 * @brief
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 03:42:16
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include "rpc_provider.h"
#include "logger.h"
#include "zookeeper_cli.h"
#include "kit_rpc_application.h"

#include <thread>
#include <google/protobuf/descriptor.h>


namespace kit_rpc {

Provide::Provide(const std::string &ip, uint16_t port, const std::string& name)
    :_server(std::make_unique<mn::TcpServer>(&_loop, mn::InetAddress(ip, port), name))
{

}

/**
* @brief 注册rpc服务+方法
* @param[in] service
*/
void Provide::notifyService(GPServicePtr service)
{
    ServiceInfo service_info;
    service_info._service = service;

    const gp::ServiceDescriptor* describtor = service->GetDescriptor();

    auto service_name = describtor->name();
    for(int i = 0;i < describtor->method_count();++i)
    {
        auto method_name = describtor->method(i)->name();
        service_info._methodsMap.insert({method_name, describtor->method(i)});
    }

    _servicesMap.insert({service_name, service_info});
}
/**
* @brief 执行rpc方法
*/
void Provide::run()
{
    _server->setThreadNum(std::thread::hardware_concurrency());

    _server->setConnectionCallback(std::bind(&Provide::onConnection, this, std::placeholders::_1));

    _server->setMessageCallback(std::bind(&Provide::onMessage, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3));

    _server->setWriteCompleteCallback([](const mn::TcpConnectionPtr& conn){
        RPC_INFO("resp ==> %s send ok \n", conn->peerAddress().toIpPort().c_str());
    });

    //获取本地ip
    auto &k = KitRpcApplication::GetInstace();
    std::string host = k.getConfig()->get("rpc_server_ip");
    host += ":";
    host += k.getConfig()->get("rpc_server_port");

    //当前本地服务全部注册到Zookeeper
    ZkClient cli;
    cli.start();
    for(auto &it : _servicesMap)
    {
        std::string path = "/";
        auto service =  it.second._service;
        RPC_INFO("%s server start!\n", service->GetDescriptor()->name().c_str());

        path += service->GetDescriptor()->name();
        if(!cli.createNode(path, ""))  //永久性节点 且无数据
        {
            RPC_ERR("zookeeper createNode error\n");
        }
        for(auto &m :  it.second._methodsMap)
        {
            std::string node_path = path;
            node_path += "/";
            node_path += m.second->name();
            if(!cli.createNode(node_path, host, ZOO_EPHEMERAL))// 临时性节点  存储发布服务的ip:port
            {
                RPC_ERR("zookeeper createNode error\n");
            }
            RPC_INFO("\t\t ===>%s \n", m.second->full_name().c_str());
        }

    }

    _server->start();
    _loop.loop();
}


void Provide::onConnection(const mn::TcpConnectionPtr& conn)
{
    RPC_INFO("Provide::onConnection status change: %s \n", conn->peerAddress().toIpPort().c_str());
}


/*
    RPC协议解析 + rpc方法寻址调用
    header_size  +  rcp_header  + args
    (固定4Byte)

*/

void Provide::onMessage(const mn::TcpConnectionPtr& conn, mn::Buffer* buffer, ::muduo::Timestamp time_stamp)
{

    std::string data = buffer->retrieveAllAsString();

    uint32_t len = 0;
    while(len < data.size())
    {
        uint32_t header_size = 0;
        data.copy((char*)&header_size, sizeof(uint32_t), len);
        len +=  sizeof(uint32_t);

        std::string head_str(header_size, 0);
        data.copy(head_str.data(), header_size, len);
        len += header_size;

        RpcHeader head;
        if(!head.ParseFromString(head_str))
        {
            RPC_ERR("parse rpc header error! header_size= %u \n", header_size);
            return;
        }

        uint32_t args_size = head.args_size();
        std::string args_str(args_size, 0);
        if(args_size > 0)
        {
            data.copy(args_str.data(), args_size, len);
            len += args_size;
        }

        handleRpcRequest(conn, head, args_str);
        /*  开线程 调用框架接口 */
        // auto th = std::thread(&Provide::handleRpcRequest, this, conn, head, args_str);
        // th.detach();

    }
}


void Provide::handleRpcRequest(const mn::TcpConnectionPtr& conn, const RpcHeader head, const std::string args_str)
{
    auto sit = _servicesMap.find(head.service_name());
    if(sit == _servicesMap.end())
    {
        RPC_ERR("%s dont exist!\n", head.service_name().c_str());
        return;
    }
    auto service_info = sit->second;
    auto mit = service_info._methodsMap.find(head.method_name());

    if(mit == service_info._methodsMap.end())
    {
        RPC_ERR("%s.%s dont exist!\n", head.service_name().c_str(), head.method_name().c_str());
        return;
    }
    GPServicePtr service = service_info._service;
    const gp::MethodDescriptor* method = mit->second;

    // 方法的请求参数
    gp::Message *request = service->GetRequestPrototype(method).New();
    // 方法的响应参数
    gp::Message *response = service->GetResponsePrototype(method).New();
    // 方法的执行完毕回调
    gp::Closure *done = gp::NewCallback<Provide, const mn::TcpConnectionPtr&, gp::Message *>(this, &Provide::sendRpcResponse, conn, response);

    if(!request->ParseFromString(args_str))
    {
        RPC_ERR("%s.%s args parse error!\n", head.service_name().c_str(), head.method_name().c_str());
        return;
    }

    service->CallMethod(method, nullptr, request, response, done);
}

void Provide::sendRpcResponse(const mn::TcpConnectionPtr& conn, gp::Message * response)
{
    // 1. 序列化Response结构体
    // 2. 送入网络buffer数据
    // 3. 发送
    std::string resp_str;
    if(!response->SerializePartialToString(&resp_str))
    {
        RPC_ERR("Serialize Response error!\n");
    }
    else
    {
        conn->send(resp_str.data(), resp_str.size());
    }

    conn->shutdown();
}

}   //kit_rpc