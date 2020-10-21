/**
*  File: utils.h
*  Desc: 无依赖的工具类
*  Created by felix on 2020/09/14
*/

#pragma once

#include <iostream>
#include <windows.h>
#include "tlhelp32.h"
#include <memory>



namespace tools {

    //1. 判断目录是否存在
    //创建目录  -p

    //2. 设置core  windows/liunux

    DWORD GetProcessidFromName(LPCTSTR name)
    {
        PROCESSENTRY32 pe;
        DWORD id = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (!Process32First(hSnapshot, &pe))
            return 0;
        while (1)
        {
            pe.dwSize = sizeof(PROCESSENTRY32);
            if (Process32Next(hSnapshot, &pe) == FALSE)
                break;
            if (std::strcmp(pe.szExeFile, name) == 0)
            {
                id = pe.th32ProcessID;

                break;
            }
        }
        CloseHandle(hSnapshot);
        return id;
    }
}