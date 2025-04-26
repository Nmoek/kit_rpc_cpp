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

namespace kit_rpc {

class KitRpcApplication
{

public:
    static void Init(int argc, char **argv);

    /**
     * @brief 获取单例的对象实例
     * @return KitRpcApplication&
     */
    static KitRpcApplication& GetInstace();

private:
    KitRpcApplication() = default;
    KitRpcApplication(const KitRpcApplication&) = delete;
    KitRpcApplication& operator=(const KitRpcApplication&) = delete;

};



}   //kit_rpc
#endif