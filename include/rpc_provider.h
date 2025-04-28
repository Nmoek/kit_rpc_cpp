/**
 * @file rpc_provider.h
 * @brief 发布rpc网络服务
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 03:28:32
 * @copyright Copyright (c) 2025 Kewin Li
 */
#ifndef __RPC_PROVIDE_H__
#define __RPC_PROVIDE_H__

#include <google/protobuf/service.h>
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"

namespace kit_rpc {

namespace gp = ::google::protobuf;
namespace mn = ::muduo::net;


class Provide
{
public:
    using GPService = gp::Service;
    using GPServicePtr = std::shared_ptr<gp::Service>;
    using MNTcpServerUPtr = std::unique_ptr<mn::TcpServer>;

    explicit Provide(const std::string &ip = "127.0.0.1", uint16_t port = 8080, const std::string& name = "unknow");

    /**
     * @brief 注册rpc方法
     * @param[in] service
     */
    void notifyService(GPServicePtr service);

    /**
     * @brief 开始对外发布rpc方法
     */
    void run();
private:

    /**
     * @brief tcp新连接回调
     * @param[in] conn 连接信息
     */
    void onConnection(const mn::TcpConnectionPtr& conn);

    /**
     * @brief tcp读写回调
     * @param conn
     * @param buffer
     * @param time_stamp
     */
    void onMessage(const mn::TcpConnectionPtr& conn, mn::Buffer* buffer, ::muduo::Timestamp time_stamp);

private:
    /// @brief 待注册rpc服务
    GPServicePtr _service;
    /// @brief 事件循环句柄
    mn::EventLoop _loop;
    /// @brief muduo tcp服务器
    MNTcpServerUPtr _server;
};




}   // kit_rpc
#endif