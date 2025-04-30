/**
 * @file rpc_controller.h
 * @brief RPC调用过程监测与控制
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-30 22:48:41
 * @copyright Copyright (c) 2025 Kewin Li
 */
#ifndef __RPC_CONTROLLER_H__
#define __RPC_CONTROLLER_H__


#include <google/protobuf/service.h>

namespace gp = ::google::protobuf;

namespace kit_rpc {

class KitRpcController: public gp::RpcController
{

public:
    KitRpcController();

    void Reset();

    bool Failed() const;

    std::string ErrorText() const;

    void SetFailed(const std::string& reason);

    /* 以下接口暂时无具体实现 */
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(gp::Closure* callback);
private:
    /// @brief 是否出错
    bool _isError;
    /// @brief 出错信息
    std::string _errMsg;
};


}   // kit_rpc
#endif