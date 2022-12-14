/**
 * @author: bo.liu
 * @mail: geniusrabbit@qq.com
 * @date: 2022.09.28
 * @brief: 配置文件管理（通用库）
 */
#ifndef UTILS_CONFIG_H
#define UTILS_CONFIG_H
#include <list>
#include <string>
#include <fstream>
#include <stdio.h>

#include "json.h"
#include "utils_log.h"
#ifdef __linux__
#include <dir.h>
#else
#include <Windows.h>
#include <sphelper.h>
#endif

class Config {
public:
    explicit Config(const std::string &config_path="./config.json"){
        init = false;
        Json::Reader reader = Json::Reader();
        m_config_path = get_abs_path() + "/" + config_path;

        if(reader.parse(load_string_from_file(m_config_path), m_config)){
            printf("config file %s load success", m_config_path.c_str());
            init = true;
        }else{
            printf("config file %s load fail", m_config_path.c_str())
        }
    }

    [[nodiscard]] bool check_init() const{
        return init;
    }

    template<typename T>
    T get(const std::string &key, const std::string &prefix="", T default_val= T()){
        if(prefix.empty()){
            return _get(m_config, key, default_val);
        }

        Json::Value prefix_root = m_config.get(prefix, Json::Value());
        return _get(prefix_root, key, default_val);
    }

    template<typename T>
    void set(const std::string &key, const std::string &prefix="", T val = T()){
        if(prefix.empty()){
            _set(m_config, key, val);
            return;
        }

        Json::Value prefix_root = m_config.get(prefix, Json::Value());
        if(prefix_root.empty()){
            // prefix 不存在，设置后重新获取
            m_config[prefix] = prefix_root;
            prefix_root = m_config.get(prefix, Json::Value());
        }
        _set(prefix_root, key, val);
    }

    template<typename T>
    std::list<T> get_list(const std::string &key, const std::string &prefix="", std::list<T> default_val= std::list<T>()){
        if(prefix.empty()){
            return _get_list(m_config, key, default_val);
        }

        Json::Value prefix_root = m_config.get(prefix, Json::Value());
        return _get_list(prefix_root, key, default_val);
    }

    template<typename T>
    void set_list(const std::string &key, const std::string &prefix="", std::list<T> val = std::list<T>()){
        if(prefix.empty()){
            _set_list(m_config, key, val);
            return;
        }

        Json::Value prefix_root = m_config.get(prefix, Json::Value());
        if(prefix_root.empty()){
            // prefix 不存在，设置后重新获取
            m_config[prefix] = prefix_root;
            prefix_root = m_config.get(prefix, Json::Value());
        }
        _set_list(prefix_root, key, val);
    }

    template<typename T>
    std::multimap<std::string, T> get_map(const std::string &key, const std::string &prefix= "", T default_val= T()){
        if(prefix.empty()){
            return _get_map(m_config, key, default_val);
        }

        Json::Value prefix_root = m_config.get(prefix, Json::Value());
        return _get_map(prefix_root, key, default_val);
    }

    template<typename T>
    void set_map(const std::string &key, const std::string &prefix= "", std::multimap<std::string, T> val_list= std::multimap<std::string, T>()){
        if(prefix.empty()){
            _set_map(m_config, key, val_list);
            return;
        }

        Json::Value prefix_root = m_config.get(prefix, Json::Value());
        if(prefix_root.empty()){
            // prefix 不存在，设置后重新获取
            m_config[prefix] = prefix_root;
            prefix_root = m_config.get(prefix, Json::Value());
        }
        _set_map(prefix_root, key, val_list);
    }

private:
    static std::string get_abs_path(){
        char path_buffer[1024];
#ifdef __linux__
        getcwd(path_buffer, 1024);
#else
        // window 获取存放路径
        ::GetModuleFileNameA(nullptr, path_buffer, 1024);
        (strrchr(path_buffer, '\\'))[1] = 0;
#endif
        return path_buffer;
    }

    static std::string load_string_from_file(const std::string &path){
        std::ifstream i_file(path);
        //将文件读入到ostringstream对象buf中
        std::ostringstream buf;
        char ch;
        while(buf && i_file.get(ch)){
            buf.put(ch);
        }
        //返回与流对象buf关联的字符串
        return buf.str();
    }

    void save_config_to_file(){
        std::ofstream os_write(m_config_path, std::ofstream::in);
        os_write << m_config.toStyledString();
        os_write.close();
    }

    template<typename T>
    T _get(const Json::Value &root, const std::string &key, T default_val= T()){
        if(key.empty() || root.empty()){
            return default_val;
        }

        return root.get(key, default_val).template as<T>();
    }

    template<typename T>
    void _set(Json::Value &root, const std::string &key, T val= T()){
        if(key.empty() || root.empty()){
            return;
        }

        root[key] = val;
        // save
        save_config_to_file();
    }

    template<typename T>
    std::list<T> _get_list(const Json::Value &root, const std::string &key, std::list<T> default_val= std::list<T>()){
        if(key.empty() || root.empty()){
            return default_val;
        }

        Json::Value value_list = root.get(key, Json::Value());
        if(value_list.empty()){
            return default_val;
        }

        std::list<T> result_list;
        for(const auto & idx : value_list){
            result_list.push_back(idx.template as<T>());
        }
        return result_list;
    }

    template<typename T>
    void _set_list(Json::Value &root, const std::string &key, std::list<T> val= std::list<T>()){
        if(key.empty() || root.empty()){
            return;
        }

        for(const auto &it : val){
            root[key].append(it);
        }

        // save
        save_config_to_file();
    }

    template<typename T>
    std::multimap<std::string, T> _get_map(const Json::Value &root, const std::string &key, T default_val= T()){
        std::multimap<std::string, T> result_list;
        Json::Value list;

        if(key.empty() || root.empty()){
            return result_list;
        }

        // 获取对应位置的值
        list = root.get(key, Json::Value());
        if(list.empty()){
            // 空值
            return result_list;
        }

        // 取数据内容
        for(const auto &it : list.getMemberNames()){
            T t = list.get(it, default_val).template as<T>();
            result_list.emplace(it, t);
        }
        return result_list;
    }

    template<typename T>
    void _set_map(Json::Value &root, const std::string &key, std::multimap<std::string, T> val_list= std::multimap<std::string, T>()){
        if(key.empty() || root.empty() || val_list.empty()){
            return;
        }

        // 生成list
        Json::Value list_value;
        for(const auto &it : val_list){
            list_value[it.first] = it.second;
        }
        // 插入到root中
        root[key] = val_list;
        // save
        save_config_to_file();
    }

private:
    bool init;
    std::string m_config_path;
    Json::Value m_config;
};

#endif //UTILS_CONFIG_H
