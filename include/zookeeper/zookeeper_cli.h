/**
 * @file zookeeper_cli.h
 * @brief zookeeper客户端
 * @author Kewin Li
 * @version 1.0
 * @date 2025-05-01 21:06:50
 * @copyright Copyright (c) 2025 Kewin Li
 */
#ifndef __ZOOKEEPER_CLI_H__
#define __ZOOKEEPER_CLI_H__

#include <zookeeper/zookeeper.h>
#include <string>

namespace kit_rpc {


class ZkClient
{
public:
    ZkClient();
    ~ZkClient();

    /**
     * @brief zookeeper客户端发起连接
     */
    void start();

    /**
     * @brief 创建数据节点
     * @param[in] path
     * @param[in] data
     * @param[in] state  0=永久节点 1=临时节点
     * @return true
     * @return false
     */
    bool createNode(const std::string& path, const std::string& data, int32_t state = 0);

    /**
     * @brief 获取路径节点上的数据
     * @param[in] path
     * @return std::string
     */
    std::string getNodeData(const std::string &path);

    /**
     * @brief 删除路径节点
     * @param[in] path
     */
    void deleteNode(const std::string &path);

private:
    zhandle_t* _handle;

};





}   //kit_rpc
#endif