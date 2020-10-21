/**
*  File: CFileReader.h
*  Desc: 提供对日志文件的实时读取 (一直读取)
*  Created by felix on 2020/09/14
*/
#include <fstream>
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <windows.h>

#pragma once

class FileReader {
    

    public:

    FileReader(const char * filepath, int lineNum, int interval);

    void RegisterLineProcessFunc(std::function<void(std::string)>);

    void StartProcess();

    void Process();

    int GetCurrentLineNum();

    private:
        bool isRunning_;
        std::function<void(std::string)> lineProcFunc_cb_;
        std::ifstream fin_;

        int currentLineNum_;

        int interval_; // milli second

        std::thread*  thread_proc_;
};