/**
 * @file rpc_provider.cpp
 * @brief
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 03:42:16
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include "rpc_provider.h"

namespace kit_rpc {

/**
* @brief 注册rpc方法
* @param[in] service
*/
void Provide::notifyService(GP_Service *service)
{
    std::cout << "Provide::notifyService" << std::endl;
}
/**
* @brief 执行rpc方法
*/
void Provide::run()
{
    std::cout << "Provide::run" << std::endl;
}


}   //kit_rpc