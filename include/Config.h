/**
*  File: Config.h
*  Desc: 配置类， 用于读取json配置文件
*  Created by felix on 2020/09/10
*/

#pragma once

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>


void load_from_json(const char * path, nlohmann::json & json);

class ZabbixConfig
{

public:
    explicit ZabbixConfig(const char *path);

    ~ZabbixConfig();

    void ConvertFromJson();


    std::string GetZbx_server();

    std::string GetZbx_Submit_Rule();

    std::string GetZbxKey();

    std::string GetZbxHostName();

    std::string GetMonitorExe();

    std::string GetMonitorLog();

    int GetZbxPort();

    int GetMonitorLineNum();

    int GetMonitorOutFlag();

    void SetMonitorLineNum(int);

    void RewriteJsonToFile();

private:
    nlohmann::json json_obj_;

    std::string monitor_exe_;
    std::string monitor_log_;
    //TODO if atomic necessary
    int monitor_lineNum_;
    std::string zbx_host_name_;
    std::string zbx_key_;
    std::string zbx_server_;
    std::string zbx_submit_rule_;
    int zbx_port_;
    std::string config_filepath_;
    int monitor_out_flag_ = 0;
};