/**
 * @file kit_rpc_application.h
 * @brief RPC框架基础类
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 03:17:07
 * @copyright Copyright (c) 2025 Kewin Li
 */

#ifndef __KIT_RPC_APPLICATION_H__
#define __KIT_RPC_APPLICATION_H__

#include "rpc_config.h"
#include "rpc_provider.h"
#include "rpc_channel.h"
#include "rpc_controller.h"
#include "logger.h"

namespace kit_rpc {

class KitRpcApplication
{

public:
    void Init(int argc, char **argv);

    /**
     * @brief 获取单例的对象实例
     * @return KitRpcApplication&
     */
    static KitRpcApplication& GetInstace();

    /**
     * @brief 设置配置项
     * @param[in] config
     */
    void setConfig(RpcConfig::Ptr config);

    /**
     * @brief 获取配置项
     * @return RpcConfig::Ptr
     */
    RpcConfig::Ptr getConfig() const;


private:
    KitRpcApplication() = default;
    KitRpcApplication(const KitRpcApplication&) = delete;
    KitRpcApplication& operator=(const KitRpcApplication&) = delete;

private:
    /// @brief rpc配置管理
    RpcConfig::Ptr _config;
    /// @brief 是否初始化
    bool _isInit;

};



}   //kit_rpc
#endif