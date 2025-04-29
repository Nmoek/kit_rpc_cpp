/**
 * @file rpc_provider.cpp
 * @brief
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 03:42:16
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include "rpc_provider.h"

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

    std::cout << "server start: " << _server->ipPort() << std::endl;

    _server->start();
    _loop.loop();
}


void Provide::onConnection(const mn::TcpConnectionPtr& conn)
{
    std::cout << "Provide::onConnection start new connection: " << conn->peerAddress().toIpPort() << std::endl;

}


void Provide::onMessage(const mn::TcpConnectionPtr& conn, mn::Buffer* buffer, ::muduo::Timestamp time_stamp)
{
    std::cout << "Provide::onMessage start: " << buffer->retrieveAllAsString() << std::endl;

}

}   //kit_rpc