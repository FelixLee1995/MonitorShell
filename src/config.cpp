#include "Config.h"

ZabbixConfig::ZabbixConfig(const char * path): config_filepath_(path) {

    load_from_json(path, json_obj_);

    ConvertFromJson();

}



ZabbixConfig::~ZabbixConfig() {

    std::cout << "析构函数" << std::endl;

}

void ZabbixConfig::RewriteJsonToFile() {

    json_obj_["monitor_lineNum"] = monitor_lineNum_;

    try
    {
        std::ofstream out(config_filepath_);
        out << json_obj_.dump(2) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "RewriteToFile error: " << e.what() << std::endl;
    }
}


void ZabbixConfig::ConvertFromJson() {
    try {
        json_obj_.at("monitor_exe").get_to(monitor_exe_);
        json_obj_.at("monitor_out").get_to(monitor_log_);
        json_obj_.at("monitor_out_flag").get_to(monitor_out_flag_);
        json_obj_.at("monitor_lineNum").get_to(monitor_lineNum_);
        json_obj_.at("host").get_to(zbx_host_name_);
        json_obj_.at("key").get_to(zbx_key_);
        json_obj_.at("zabbix_server").get_to(zbx_server_);
        json_obj_.at("zabbix_submit_rule").get_to(zbx_submit_rule_);
        json_obj_.at("zabbix_port").get_to(zbx_port_);
    }
    catch(nlohmann::detail::type_error e){
        std::cout << "json parse error: " << e.what() << std::endl;
    }
}


void load_from_json(const char * path, nlohmann::json & json){

    try {
        std::ifstream i(path);
        if (!i) {
            std::cout << "config file path error!" << std::endl;
            return;
        } 
        i >> json;
    }
    catch(std::exception e) {
        std::cout << e.what() << std::endl;
    }
}


int ZabbixConfig::GetMonitorOutFlag(){
    return monitor_out_flag_;
}



std::string ZabbixConfig::GetZbx_server()
{
    return zbx_server_;
}
std::string ZabbixConfig::GetZbx_Submit_Rule()
{
    return zbx_submit_rule_;
}
std::string ZabbixConfig::GetZbxKey()
{
    return zbx_key_;
}
std::string ZabbixConfig::GetZbxHostName()
{
    return zbx_host_name_;
}
std::string ZabbixConfig::GetMonitorExe()
{
    return monitor_exe_;
}

std::string ZabbixConfig::GetMonitorLog() {
    return monitor_log_;
}

int ZabbixConfig::GetZbxPort() {
    return zbx_port_;
}

int ZabbixConfig::GetMonitorLineNum() {
    return monitor_lineNum_;
}

void ZabbixConfig::SetMonitorLineNum(int num) {
    monitor_lineNum_ = num;
}
