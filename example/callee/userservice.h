/**
 * @file userservice.cpp
 * @brief
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-26 21:25:29
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include <iostream>
#include <string>

#include "user.pb.h"

namespace gp = ::google::protobuf;

/**
 * @brief 对外提供UserService
 */
class UserService: public kit_rpc::UserServiceRpc
{
public:
    bool Login(std::string name ,std::string pwd)
    {
        std::cout << "local Login service ...." << std::endl;
        std::cout << "name= " << name << ", pwd= " << pwd << std::endl;

        return true;
    }

    bool LogOut(std::string name)
    {
        std::cout << "local LogOut service ...." << std::endl;
        std::cout << "name= " << name << std::endl;

        return false;
    }

    void Login(
        ::gp::RpcController* controller,
        const ::kit_rpc::LoginRequest* request,
        ::kit_rpc::LoginResponse* response,
        ::gp::Closure* done)
    {
        std::cout << "rpc Login service ...." << std::endl;

        std::string name = request->name();
        std::string pwd = request->pwd();

        bool success = Login(name, pwd);

        auto result = response->mutable_result();
        result->set_errcode(0);
        result->set_errmsg("Login OK");
        response->set_success(success);

        // protobuf回调函数
        if(done)
            done->Run();
    }

    virtual void LogOut(
        ::gp::RpcController* controller,
        const ::kit_rpc::LogOutRequest* request,
        ::kit_rpc::LogOutResponse* response,
        ::gp::Closure* done)
    {
        std::cout << "rpc LogOut service ...." << std::endl;

        std::string name = request->name();

        bool success = LogOut(name);

        auto result = response->mutable_result();
        result->set_errcode(11);
        result->set_errmsg("LogOut Faild");
        response->set_success(success);

        // protobuf回调函数
        if(done)
            done->Run();
    }

};