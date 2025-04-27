/**
 * @file kit_rpc_application.cpp
 * @brief RPC框架基础类
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 03:25:03
 * @copyright Copyright (c) 2025 Kewin Li
 */

#include "kit_rpc_application.h"

#include <unistd.h>
#include <iostream>

namespace kit_rpc {


static void ShowArgsHelp()
{
    std::cout << "help: commond -i <configfile> " << std::endl;
}



void KitRpcApplication::Init(int argc, char **argv)
{
    std::cout << "KitRpcApplication::Init" << std::endl;
    if(argc < 2)
    {
        ShowArgsHelp();
        exit(-1);
    }

    char c = 0;
    while((c = ::getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
            case 'i':
            {
                std::string config_file = ::optarg;

                std::cout << "config_file= " << config_file << std::endl;
                break;
            }
            // 读到非法字符
            case '?':
            {
                std::cerr << "invalid args" << std::endl;
                ShowArgsHelp();
                exit(-1);
            }
            // 读到"-i" 但没有config文件
            case ':':
            {
                std::cerr << "dont find <configfile>" << std::endl;
                ShowArgsHelp();
                exit(-1);
            }
            default:
                ShowArgsHelp();
                exit(-1);
        }
    }

    // 读取配置文件并解析

}

KitRpcApplication& KitRpcApplication::GetInstace()
{
    static KitRpcApplication k;
    return k;
}



} // kit_rpc