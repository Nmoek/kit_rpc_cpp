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

    _f.open(_configFile, std::ios::in | std::ios::binary);
    if(!_f.is_open())
    {
        std::cerr << _configFile << " open error, "
            << errno << ":" << strerror(errno) << std::endl;
        return false;
    }

    _configParse = s_configFileType[_type]();

    return true;
}

bool RpcConfig::loadConfigFromFile()
{
    if(!_f.is_open())
        return false;

    // 一次性读取然后解析
    _f.seekg(0, std::ios::end);
    int64_t file_size = _f.tellg();
    _f.seekg(0, std::ios::beg);

    std::string file_data(file_size, 0);
    if(!_f.read(file_data.data(), file_size))
    {
        std::cerr << _configFile << " read error!"
            << errno << ":" << strerror(errno) << std::endl;
        return false;
    }

    if(!_configParse->parse(file_data.data(), _configItem))
    {
        std::cerr << "config load fail!" << std::endl;
        return false;
    }

    std::cout << _configFile << " parse success! size= " << file_size << std::endl;

    return true;
}

bool RpcConfig::load()
{
    return loadFile() ? loadConfigFromFile() : false ;
}

}   //kit_rpc
