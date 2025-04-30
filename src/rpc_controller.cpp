/**
 * @file rpc_controller.cpp
 * @brief RPC调用过程监测与控制
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-30 22:54:11
 * @copyright Copyright (c) 2025 Kewin Li
 */

#include "rpc_controller.h"

namespace kit_rpc {


KitRpcController::KitRpcController()
    :_isError(false)
{

}

void KitRpcController::Reset()
{
    _isError = false;
    _errMsg.clear();
}

bool KitRpcController::Failed() const
{
    return _isError;
}

std::string KitRpcController::ErrorText() const
{
    return _errMsg;
}

void KitRpcController::SetFailed(const std::string& reason)
{
    _isError = true;
    _errMsg = reason;
}

void KitRpcController::StartCancel() { }
bool KitRpcController::IsCanceled() const { return false; }
void KitRpcController::NotifyOnCancel(gp::Closure* callback) { }

}   //kit_prc