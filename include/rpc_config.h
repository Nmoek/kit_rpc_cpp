/**
 * @file rpc_config.h
 * @brief RPC相关配置加载
 * @author Kewin Li
 * @version 1.0
 * @date 2025-04-27 21:23:43
 * @copyright Copyright (c) 2025 Kewin Li
 */

#ifndef __RPC_CONFIG_H__
#define __RPC_CONFIG_H__

#include <string>
#include <boost/any.hpp>
#include <fstream>
#include <functional>

#include "rpc_config_parse.h"

namespace kit_rpc {


class RpcConfig
{
public:
    using ParseTypeWrap = std::function<RpcConfigParse::UPtr()>;

    enum FileType {
        UNKNOW_FILE = 0,
        INI_FILE       ,
        JSON_FILE      ,
        YAML_FILE      ,
    };

    RpcConfig(const std::string& file, FileType type);

    RpcConfig(const char* file, FileType type);

    ~RpcConfig();

    void setFile(const std::string& file, FileType type);

    std::string getFile() const;

    bool loadFile();

    void loadConfigFromFile();

    void set(const std::string& key, boost::any &val);
    boost::any get(const std::string& key) const;

private:
    RpcConfig::FileType checkFileType();

public:
    static std::unordered_map<FileType, ParseTypeWrap> s_configFileType;
private:
    /// @brief 文件流句柄
    std::fstream _f;
    /// @brief 配置文件路径
    std::string _configFile;
    /// @brief 配置文件类型
    FileType _type;
    /// @brief 已经读取到的配置项
    std::unordered_map<std::string, boost::any> _configItem;
    /// @brief 配置解析器
    RpcConfigParse::UPtr _configParse;
};




}   //kit_rpc
#endif