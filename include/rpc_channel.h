/**
 * @file rpc_channel.h
 * @brief RPC发起调用
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-30 19:57:31
 * @copyright Copyright (c) 2025 Kewin Li
 */
#ifndef __RPC_CHANNEL_H__
#define __RPC_CHANNEL_H__


#include <google/protobuf/service.h>

namespace gp = ::google::protobuf;

namespace kit_rpc {


class KitRpcChannel: public gp::RpcChannel
{

public:

    void CallMethod(
        const gp::MethodDescriptor* method,
        gp::RpcController* controller,
        const gp::Message* request,
        gp::Message* response,
        gp::Closure* done);


private:


};




}   //kit_rpc
#endif