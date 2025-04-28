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
    using Ptr = std::shared_ptr<RpcConfig>;
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

    bool load();

    template<typename T>
    void set(const std::string& key, T &&val)
    {
        _configItem[key] = std::move(std::forward<T>(val));
    }

    template<typename T = std::string>
    T get(const std::string& key) const
    {
        auto it = _configItem.find(key);
        if(it == _configItem.end())
            return  T();

        // 指针转换 避免抛异常
        auto p = boost::any_cast<T>(&(it->second));
        if(!p)  //提取失败
        {
            auto p2 = boost::any_cast<std::string>(&(it->second));
            if(!p2)
            {
                std::cerr << "src type is not string!" << std::endl;
                return T();
            }
            // 目前仅支持数字转换
            // 陷阱: 需要在编译阶段判断的条件 最好都加 constexpr
            if constexpr (std::is_integral_v<T>)
            {
                return static_cast<T>(std::stoi(*p2));
            }
            else if constexpr(std::is_floating_point_v<T>)
            {
                return static_cast<T>(std::stof(*p2));
            }
            else
            {
                std::cerr << "data type is invalid!" << std::endl;
                return T();
            }

        }

        return *p;
    }

    auto get(const std::string& key) const
    {
        return get<>(key);
    }
private:
    bool loadFile();

    bool loadConfigFromFile();

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