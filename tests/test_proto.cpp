/**
 * @file test_proto.cpp
 * @brief 测试protoc指令
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-24 04:52:49
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include "./test.pb.h"
#include <iostream>
#include <gtest/gtest.h>

using namespace fixbug;

TEST(TestProto, rquest)
{
    Request req;
    req.set_name("ljk");
    req.set_pwd("666");
    req.set_addr("yunnan");

    std::string encodeString;
    if(req.SerializePartialToString(&encodeString))
    {
        std::cout << "encodeString= " << encodeString.c_str() << std::endl;
    }

    Request req2;
    if(req2.ParseFromString(encodeString))
    {
        std::cout << "name= " << req2.name() << std::endl;
        std::cout << "pwd= " << req2.pwd() << std::endl;

    }

}

TEST(TestProto, Response)
{
    Response rsp;
    auto err = rsp.mutable_err();
    err->set_code(Error_ErrorCode_OK);
    err->set_reson("OK");
    rsp.set_pwd("pwd");
    rsp.set_is_online(true);
    auto qunzu = rsp.mutable_qunzu();
    qunzu->insert({1, "1"});
    qunzu->insert({2, "2"});
    /********res[0]*****/
    auto res = rsp.add_res();
    std::cout << "repeated res size= " << rsp.res_size() << std::endl;
    res->set_id(1);

    /********res[1]*****/
    res = rsp.add_res();
    std::cout << "repeated res size= " << rsp.res_size() << std::endl;
    res->set_id(2);

    std::string encodeString;
    if(rsp.SerializePartialToString(&encodeString))
    {
        std::cout << "encodeString= " << encodeString.c_str() << std::endl;
    }

    std::cout << "----------------------\n";

    Response rsp2;
    if(rsp2.ParsePartialFromString(encodeString))
    {
        std::cout <<"Error:" << std::endl
            << "\t" << "code= " << rsp2.err().code() << std::endl
            << "\t" << "reson= " << rsp2.err().reson() << std::endl;
        std::cout << "pwd: " << rsp2.pwd() << std::endl;
        std::cout << "is_online: " << rsp2.is_online() << std::endl;

        std::cout << "map qunzu:" << std::endl;
        for(auto it : rsp2.qunzu())
        {
            std::cout << "\t" <<  it.first << " : " << it.second << std::endl;
        }

        std::cout << "repeated Result:" << std::endl;
        auto ress = rsp2.res();
        for(int i = 0;i < ress.size();++i)
        {
            std::cout << "res[" << i << "]: " << std::endl
                << "\t" << "id= " << ress[i].id() << std::endl;
        }


    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}