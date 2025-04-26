/**
 * @file kit_rpc_application.cpp
 * @brief RPC框架基础类
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 03:25:03
 * @copyright Copyright (c) 2025 Kewin Li
 */

#include "kit_rpc_application.h"

#include <iostream>

namespace kit_rpc {

void KitRpcApplication::Init(int argc, char **argv)
{
    std::cout << "KitRpcApplication::Init" << std::endl;
}

KitRpcApplication& KitRpcApplication::GetInstace()
{
    static KitRpcApplication k;
    return k;
}



} // kit_rpc