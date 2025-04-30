/**
 * @file logger.cpp
 * @brief RPC框架日志系统
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-30 23:23:49
 * @copyright Copyright (c) 2025 Kewin Li
 */

#include "logger.h"
#include <sys/time.h>
#include <time.h>
#include <cstring>
#include <fcntl.h>

namespace kit_rpc {

Logger::Logger()
{
    std::thread writeTask([this](){
        char timeBuf[32] = {0};
        char fileName[64] = {0};
        for(;;)
        {
            time_t t = time(nullptr);
            struct tm *tm = localtime(&t);
            memset(timeBuf, 0, sizeof(timeBuf));
            strftime(timeBuf, sizeof(timeBuf),"%Y-%m-%d", tm);

            memset(fileName, 0, sizeof(fileName));
            snprintf(fileName, sizeof(fileName), "%s/%s_rpc.log", getPath().c_str(), timeBuf);

            FILE *f = fopen(fileName, "a+");
            if(!f)
            {
                printf("%s open error! %d :%s \n", fileName, errno, strerror(errno));
                exit(-1);
            }

            auto msg = _logQue.pop();
            if(msg.empty())
                break;

            fwrite(msg.data(), msg.size(), 1, f);
            fflush(f);
            fclose(f);
        }
    });
    writeTask.detach();

}

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

void Logger::setLevel(const LogLevel::Level level)
{
    if(level < LogLevel::UNKNOW || level > LogLevel::ERROR)
        return;
    _level = level;
}

LogLevel::Level Logger::getLevel() const
{
    return _level;
}

void Logger::setPath(const std::string &path)
{
    _logPath = path;
}

std::string Logger::getPath() const
{
    return _logPath;
}

void Logger::log(const std::string &msg)
{
    _logQue.push(msg);
}

}   //kit_rpc