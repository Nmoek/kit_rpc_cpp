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


namespace kit_rpc {


class Provide
{
public:
    using GP_Service = ::google::protobuf::Service;

    /**
     * @brief 注册rpc方法
     * @param[in] service
     */
    void notifyService(GP_Service *service);

    /**
     * @brief 执行rpc方法
     */
    void run();
};




}   // kit_rpc
#endif