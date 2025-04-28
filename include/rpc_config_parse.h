/**
 * @file rpc_config_parse.h
 * @brief  配置文件解析器
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-28 01:36:01
 * @copyright Copyright (c) 2025 Kewin Li
 */
#ifndef __RPC_CONFIG_PASER_H__
#define __RPC_CONFIG_PASER_H__

#include <memory>
#include <unordered_map>
#include <boost/any.hpp>
#include <iostream>

namespace kit_rpc {

class RpcConfigParse
{

public:
    using Ptr = std::shared_ptr<RpcConfigParse>;
    using UPtr = std::unique_ptr<RpcConfigParse>;

    virtual bool parse(const std::string &data, std::unordered_map<std::string, boost::any> &config_items) = 0;

};

class IniConfigParse: public RpcConfigParse
{

public:
    bool parse(const std::string &data, std::unordered_map<std::string, boost::any> &config_items);

};

class JsonConfigParse: public RpcConfigParse
{
public:
    bool parse(const std::string &data, std::unordered_map<std::string, boost::any> &config_items)
    {
        std::cout << "JsonConfigParse::parse()" << std::endl;
        return 1;
    }
};

class YamlConfigParse: public RpcConfigParse
{
public:
    bool parse(const std::string &data, std::unordered_map<std::string, boost::any> &config_items)
    {
        std::cout << "YamlConfigParse::parse()" << std::endl;
        return 1;
    }
};


}   //kit_rpc
#endif