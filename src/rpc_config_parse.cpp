/**
 * @file rpc_config_parse.cpp
 * @brief 配置文件解析器
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-28 12:51:30
 * @copyright Copyright (c) 2025 Kewin Li
 */

#include "rpc_config_parse.h"

#include <sstream>

namespace kit_rpc {

static void TrimAllSpace(std::string &s)
{
    auto pos = s.find_first_not_of(" ");
    auto pos2 = s.find_first_of(" ", pos);
    pos2 = pos2 == -1 ? s.size() : pos2;

    s = s.substr(pos, pos2 - pos);
}

/************IniConfigParse************* */
bool IniConfigParse::parse(const std::string &data, std::unordered_map<std::string, boost::any> &config_items)
{
    if(data.empty())
        return false;

    std::stringstream ss(std::move(data));
    std::string line;

    while(std::getline(ss, line))
    {
        if(line == "\n" || line == "\r\n")
        {
            line.clear();
            continue;
        }

        auto pos = line.find_first_not_of(" ");
        if(pos == -1
            || line[pos] == '#')
        {
            line.clear();
            continue;
        }

        auto pos2 = line.find("=");
        if(pos2 == -1)
        {
            line.clear();
            continue;
        }
        auto key = line.substr(0, pos2);
        if(key.back() == '\n'
            || key.back() == '\r')
            key.pop_back();
        if(key.empty())
            continue;
        TrimAllSpace(key);
        auto val = line.substr(pos2 + 1);
        if(val.back() == '\n'
        || val.back() == '\r')
            val.pop_back();
        if(!val.empty())
            TrimAllSpace(val);

        config_items.insert({key, val});
        line.clear();
    }

    return config_items.empty() ? false : true;
}




}   //kit_rpc