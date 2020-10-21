#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <windows.h>

#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging




int main() {

    std::cerr << "Hello World cerr" << std::endl;
    std::cout << "hello world cout" << std::endl;

    std::string issue = "disconnected!";
    std::string desc = "timeout after 15 seconds";
    std::string severity = "FATAL";
    std::string monitor_flag = "MONITORFLAG";

    auto my_logger = spdlog::basic_logger_mt("monitor", "monitor.log");
    my_logger->set_pattern("%D %T@%v");

    my_logger->info("{}@{}@{}@{}", issue, desc, severity, monitor_flag);

    my_logger->flush();



    int i = 0;

    std::thread* t = new std::thread([&](){
        while(true) {
            std::cout << " i = " << i++ << std::endl;
            std::this_thread::sleep_for (std::chrono::seconds(1)); 
        }

    });

    getchar();

    return 0;
}