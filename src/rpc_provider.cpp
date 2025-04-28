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

namespace kit_rpc {

Provide::Provide(const std::string &ip, uint16_t port, const std::string& name)
    :_server(std::make_unique<mn::TcpServer>(&_loop, mn::InetAddress(ip, port), name))
{

}

/**
* @brief 注册rpc方法
* @param[in] service
*/
void Provide::notifyService(GPServicePtr service)
{
    _service = service;
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