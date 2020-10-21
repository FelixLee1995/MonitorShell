/**
*  File: ExecProxy.h
*  Desc: 控制程序的代理启动
*  Created by felix on 2020/09/11
*/

#pragma once

#include <functional>
#include <array>
#include <iostream>
#include <memory>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "color.hpp"


#define BUFSIZE 4096 
 
HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;
 
//void CreateChildProcess(const char *); 
void WriteToPipe(void); 
void ReadFromPipe(void); 
void ErrorExit(const char * str) {
    std::cout << str << std::endl;
    //exit(-1);
};



const int OP_OK = 0;



LPCWSTR CharToWCHAR(const char * src) {

    WCHAR wsz[128];
    swprintf(wsz, L"%S", src);
    LPCWSTR dest = wsz;
    return dest;
} 



void exec(const char* cmd) {
    std::array<char, 256> buffer;
    int return_code = -1;
    auto pclose_wrapper = [&return_code](FILE* cmd) { return_code = _pclose(cmd); };
    { // scope is important, have to make sure the ptr goes out of scope first
        const std::unique_ptr<FILE, decltype(pclose_wrapper)> pipe(_popen(cmd, "r"), pclose_wrapper);
        if (!pipe)
        {
            std::cerr << "Failed to execute start cmdline: " << cmd << std::endl;
            return;
        }

        // while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        // {
        //     auto ret = check_and_send_func(buffer.data());
        //     if (ret != OP_OK){
        //         std::cout<< buffer.data();
        //     }
        //     else {
        //         std::cout<< hue::red << buffer.data() << hue::reset;
        //     }
        // }
    }
}



void CreateMyProcess(const char* exe_, std::function<int(const std::string &)> check_and_send_func) {

    SECURITY_ATTRIBUTES saAttr;

    printf("\n->Start of parent execution.\n");

    // Set the bInheritHandle flag so pipe handles are inherited.

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT.

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        ErrorExit("StdoutRd CreatePipe");

    // Ensure the read handle to the pipe for STDOUT is not inherited.

    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        ErrorExit("Stdout SetHandleInformation");

    // Create a pipe for the child process's STDIN.

    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
        ErrorExit("Stdin CreatePipe");

    // Ensure the write handle to the pipe for STDIN is not inherited.

    if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
        ErrorExit("Stdin SetHandleInformation");

    // Create the child process.

    //CreateChildProcess(exe_);

    //ReadFromPipe();
}

// void CreateChildProcess(const char * exe_)
// // Create a child process that uses the previously created pipes for STDIN and STDOUT.
// { 
// //    auto szCmdline= CharToWCHAR(exe_);
// //    PROCESS_INFORMATION piProcInfo; 
// //    STARTUPINFO siStartInfo;
// //    BOOL bSuccess = FALSE; 
 
// // // Set up members of the PROCESS_INFORMATION structure. 
 
// //    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
// // // Set up members of the STARTUPINFO structure. 
// // // This structure specifies the STDIN and STDOUT handles for redirection.
 
// //    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
// //    siStartInfo.cb = sizeof(STARTUPINFO); 
// //    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
// //    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
// //    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
// //    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
// //    siStartInfo.wShowWindow = TRUE;
 
// // Create the child process. 
    
//     STARTUPINFO si = { sizeof(si) }; 
// 	PROCESS_INFORMATION pi; 
 
// 	si.dwFlags = STARTF_USESHOWWINDOW; 
// 	si.wShowWindow = FALSE; //TRUE表示显示创建的进程的窗口
// 	BOOL bRet = ::CreateProcess( 
// 		CharToWCHAR(exe_),
// 		NULL, //在Unicode版本中此参数不能为常量字符串，因为此参数会被修改	 
// 		NULL, 
// 		NULL, 
// 		FALSE, 
// 		CREATE_NEW_CONSOLE, 
// 		NULL, 
// 		NULL, 
// 		&si, 
// 		&pi); 

   
// //    // If an error occurs, exit the application. 
// //    if ( ! bSuccess ) 
// //       ErrorExit("CreateProcess");
// //    else 
// //    {
// //       // Close handles to the child process and its primary thread.
// //       // Some applications might keep these handles to monitor the status
// //       // of the child process, for example. 

// //       CloseHandle(piProcInfo.hProcess);
// //       CloseHandle(piProcInfo.hThread);
      
// //       // Close handles to the stdin and stdout pipes no longer needed by the child process.
// //       // If they are not explicitly closed, there is no way to recognize that the child process has ended.
      
// //       CloseHandle(g_hChildStd_OUT_Wr);
// //       CloseHandle(g_hChildStd_IN_Rd);
// //    }
// }

void ReadFromPipe(void) 

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{ 
   DWORD dwRead, dwWritten; 
   CHAR chBuf[BUFSIZE]; 
   BOOL bSuccess = FALSE;
   HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

   for (;;) 
   { 
      bSuccess = ReadFile( g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
      if( ! bSuccess || dwRead == 0 ) break; 

      std::cout << "read msg" <<std::endl;

      bSuccess = WriteFile(hParentStdOut, chBuf, 
                           dwRead, &dwWritten, NULL);
      if (! bSuccess ) break; 
   } 
} 


std::string  ShellExec(const char * command) {

    FILE * fPipe = _popen(command, "r");
    if (!fPipe) {
        return "error";
    }

    char buffer[128];

    while(!feof(fPipe)) {
        if (fgets(buffer, 128, fPipe) != NULL) {
            std::cout << " msg is " << buffer << std::endl;
        }
    }

    _pclose(fPipe);
    return "";
}