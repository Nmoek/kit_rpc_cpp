/**
 * @file rpc_config.cpp
 * @brief RPC相关配置加载
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 21:54:20
 * @copyright Copyright (c) 2025 Kewin Li
 */
#include "rpc_config.h"
#include <iostream>
#include <sstream>

namespace kit_rpc {

std::unordered_map<RpcConfig::FileType, RpcConfig::ParseTypeWrap> RpcConfig::s_configFileType = {
#define XX(TYPE, PARSE) \
    {TYPE, [](){ return std::make_unique<PARSE>(); }}

    XX(INI_FILE, IniConfigParse),
    XX(JSON_FILE, JsonConfigParse),
    XX(YAML_FILE, YamlConfigParse),
#undef XX
};

RpcConfig::RpcConfig(const std::string& file, RpcConfig::FileType type)
    :_configFile(file)
    ,_type(type)
{ }

RpcConfig::RpcConfig(const char* file, RpcConfig::FileType type)
    :_configFile(file)
    ,_type(type)
{}

RpcConfig::~RpcConfig()
{
    if(_f.is_open())
        _f.close();

}

void RpcConfig::setFile(const std::string& file, RpcConfig::FileType type)
{
    _f.close();
    _configFile = file;
    _type = type;
}

std::string RpcConfig::getFile() const
{
    return _configFile;
}

bool RpcConfig::loadFile()
{
    if(_f.is_open())
        _f.close();

    // 判断是否是支持的文件类型 ini、json、yaml
    if(UNKNOW_FILE >= _type || _type > YAML_FILE)
    {
        std::cerr << "file type is invalid!" << std::endl;
        return false;
    }

    _configParse = std::move(s_configFileType[_type]());

    //TODO 拿到文件类型
    _f.open(_configFile, std::ios::in | std::ios::binary);
    if(!_f.is_open())
    {
        std::cerr << _configFile << " open error, "
            << errno << ":" << strerror(errno) << std::endl;
        return false;
    }


    return true;
}

void RpcConfig::loadConfigFromFile()
{
    if(!_f.is_open())
        return;

    std::string line;


    /*TODO: 按不同文本格式来解析 这里暂时写死按照顾ini格式 */
    while(std::getline(_f, line))
    {
        if(line.size())
        {
            //文本解析
            _configParse->parse(line.data(), _configItem);
        }

        if(_f.eof() || _f.bad())
            break;
    }

}

void RpcConfig::set(const std::string& key, boost::any &val)
{
    _configItem[key] = std::move(val);
}

boost::any RpcConfig::get(const std::string& key) const
{
    auto it = _configItem.find(key);
    return it == _configItem.end() ? boost::any() : it->second;
}


}   //kit_rpc
