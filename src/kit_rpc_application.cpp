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
    if(argc < 2)
    {
        ShowArgsHelp();
        exit(-1);
    }

    std::string config_file;
    char c = 0;
    while((c = ::getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
            case 'i':
            {
                config_file = ::optarg;
                break;
            }
            // 读到非法字符
            case '?':
            {
                ShowArgsHelp();
                exit(-1);
            }
            // 读到"-i" 但没有config文件
            case ':':
            {
                ShowArgsHelp();
                exit(-1);
            }
            default:
                ShowArgsHelp();
                exit(-1);
        }
    }

    // 读取配置文件并解析
    _config = std::make_shared<RpcConfig>(config_file, RpcConfig::INI_FILE);

    if(!_config->load())
    {
        std::cerr << "rpc config load fail! \n" << std::endl;
        exit(-1);
    }

    // 初始化日志路径
    auto log_path = _config->get("log_dir");
    LOGGER().setPath(log_path.size() ? log_path : "../log/");
    _isInit = true;
}

KitRpcApplication& KitRpcApplication::GetInstace()
{
    static KitRpcApplication k;
    return k;
}

void KitRpcApplication::setConfig(RpcConfig::Ptr config)
{
    if(!_isInit)
        return;
    _config = config;
    if(!_config->load())
        RPC_ERR("rpc config load fail! \n");
}

RpcConfig::Ptr KitRpcApplication::getConfig() const
{
    if(!_isInit)
        return nullptr;
    return _config;
}

} // kit_rpc