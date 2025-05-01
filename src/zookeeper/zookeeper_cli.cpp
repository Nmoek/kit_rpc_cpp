/**
 * @file zookeeper_cli.cpp
 * @brief zookeeper客户端
 * @author Kewin Li
 * @version 1.0
 * @date 2025-05-01 21:13:14
 * @copyright Copyright (c) 2025 Kewin Li
 */

#include "zookeeper_cli.h"
#include "kit_rpc_application.h"
#include <semaphore.h>

const int32_t ZK_RECV_TIMEOUT_MS = 30000;

namespace kit_rpc {

static void watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
    //会话处理
    if(ZOO_SESSION_EVENT == type)
    {

        if(state == ZOO_CONNECTED_STATE)
        {
            RPC_INFO("zookeeper client connect success! \n");
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}


ZkClient::ZkClient()
    :_handle(nullptr)
{

}

ZkClient::~ZkClient()
{
    if(_handle)
        zookeeper_close(_handle);
}

void ZkClient::start()
{
    auto &k = KitRpcApplication::GetInstace();
    std::string host = k.getConfig()->get("zookeeper_ip");
    host += ":";
    host += k.getConfig()->get("zookeepr_port");

    std::cout << "zoo host " << host << std::endl;

    /*
        zookeeper客户端多线程版本,三个线程
        API操作线程
        网络IO线程  poll
        watcher线程
    */
    _handle = zookeeper_init(host.c_str(), watcher, ZK_RECV_TIMEOUT_MS, nullptr, nullptr, 0);
    if(nullptr == _handle)
    {
        RPC_ERR("zookeeper_init error! \n");
        exit(-1);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 3;

    zoo_set_context(_handle, &sem);
    if(sem_timedwait(&sem, &ts) < 0)
    {
        RPC_ERR("zookeeper client connect timeout!\n");
        exit(-1);
    }
}

bool ZkClient::createNode(const std::string& path, const std::string& data, int32_t state)
{
    char path_buffer[128] = {0};
    int32_t res;

    res = zoo_exists(_handle, path.c_str(), 0, nullptr);
    if(ZNONODE == res)
    {
        res = zoo_create(_handle, path.c_str(), data.c_str(), data.size(), &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, sizeof(path_buffer));
        if(ZOK != res)
        {
            RPC_ERR("zookeeper create path node err! path= %s\n", path.c_str());
            return false;
        }
        RPC_INFO("zookeeper create path node success! %s\n", path.c_str());
    }

    return true;
}

std::string ZkClient::getNodeData(const std::string &path)
{
    char path_buffer[128] = {0};
    int32_t res = 0;
    int real_buffer_size = sizeof(path_buffer);

    res = zoo_exists(_handle, path.c_str(), 0, nullptr);
    if(ZNONODE == res)
    {
        RPC_ERR("zookeeper path node not exists! %s \n", path.c_str());
        return "";
    }


    res = zoo_get(_handle, path.c_str(), 0, path_buffer, &real_buffer_size, nullptr);
    if(ZOK != res)
    {
        RPC_ERR("zookeeper get path data error! res=%d, %s \n", res, path.c_str());
        return "";
    }
    return path_buffer;
}


void ZkClient::deleteNode(const std::string &path)
{
    int32_t res = zoo_delete(_handle, path.c_str(), 0);
    if(ZOK != res)
    {
        RPC_ERR("zookeeper delete path node error! %d\n", res);
    }
}


}   //kit_rpc