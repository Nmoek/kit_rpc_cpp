/**
 * @file logger.h
 * @brief RPC框架日志系统
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-30 23:21:21
 * @copyright Copyright (c) 2025 Kewin Li
 */
#ifndef __RPC_LOGGER_H__
#define __RPC_LOGGER_H__

#include "log_queue.h"

#include <string>

#define RPC_INFO(format, ...) \
do { \
    auto& logger = kit_rpc::Logger::GetInstance(); \
    logger.setLevel(kit_rpc::LogLevel::INFO); \
    char timeBuf[32] = {0}; \
    time_t t = time(nullptr); \
    struct tm *tm = localtime(&t); \
    memset(timeBuf, 0, sizeof(timeBuf)); \
    strftime(timeBuf, sizeof(timeBuf),"%Y-%m-%d %H:%M:%S", tm); \
    char buf[1024] = {0}; \
    char formatBuf[512] = {0}; \
    snprintf(formatBuf, 512, format, ## __VA_ARGS__); \
    snprintf(buf, 1024,  "[%s][INFO][%s #%d] %s",timeBuf, __FILE__, __LINE__, formatBuf); \
    printf("%s", buf); \
    logger.log(buf); \
}while(0)

#define RPC_ERR(format, ...) \
do { \
    auto& logger = kit_rpc::Logger::GetInstance(); \
    logger.setLevel(kit_rpc::LogLevel::ERROR); \
    char timeBuf[32] = {0}; \
    time_t t = time(nullptr); \
    struct tm *tm = localtime(&t); \
    memset(timeBuf, 0, sizeof(timeBuf)); \
    strftime(timeBuf, sizeof(timeBuf),"%Y-%m-%d %H:%M:%S", tm); \
    char buf[1024] = {0}; \
    char formatBuf[512] = {0}; \
    snprintf(formatBuf, 512, format, ## __VA_ARGS__); \
    snprintf(buf, 1024,  "[%s][ERROR][%s #%d] %s",timeBuf, __FILE__, __LINE__, formatBuf); \
    printf("%s", buf); \
    logger.log(buf); \
}while(0)

#define LOGGER() kit_rpc::Logger::GetInstance()

namespace kit_rpc {

class LogLevel
{
public:
    enum Level {
        UNKNOW = 0,
        INFO      ,
        ERROR     ,
    };
};

class Logger
{
public:

    ~Logger() = default;

public:
    using QueType = LogQueue<std::string>;

    static Logger& GetInstance();

    void setLevel(const LogLevel::Level level);

    LogLevel::Level getLevel() const;

    void setPath(const std::string &path);

    std::string getPath() const;

    void log(const std::string &msg);

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    /// @brief 日志异步缓存队列
    QueType _logQue;
    /// @brief 当前日志等级
    LogLevel::Level _level;
    /// @brief 日志文件路径
    std::string _logPath;

};


}   //kit_rpc
#endif