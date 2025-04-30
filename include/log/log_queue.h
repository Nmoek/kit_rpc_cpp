/**
 * @file log_queue.h
 * @brief 日志缓冲异步队列
 * @author Kewin Li
 * @version 1.0
 * @date 2025-05-01 00:07:17
 * @copyright Copyright (c) 2025 Kewin Li
 */
#ifndef __RPC_LOG_QUEUE_H__
#define __RPC_LOG_QUEUE_H__

#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace kit_rpc {

class Logger;
template<class T>
class LogQueue
{
public:
    using MutexType = std::mutex;
    using RdLock = std::unique_lock<std::mutex>;
    using WrLock = std::unique_lock<std::mutex>;

    LogQueue() = default;

    ~LogQueue()
    {
        _isExit = true;
        _notEmpty.notify_one();
    }

    void push(const T& msg)
    {
        WrLock lock(_mutex);
        _que.push(msg);
        lock.unlock();
        _notEmpty.notify_one();
    }

    T pop()
    {
        WrLock lock(_mutex);
        _notEmpty.wait(lock, [this](){
            return _que.size() || _isExit;
        });
        if(_isExit)
            return T();
        T msg = _que.front();
        _que.pop();
        return msg;
    }

private:
    std::queue<T> _que;
    MutexType _mutex;
    std::condition_variable _notEmpty;
    bool _isExit{false};
};



}   //kit_rpc
#endif