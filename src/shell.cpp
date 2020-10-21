#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "ZbxSender.h"
#include "ExecProxy.h"
#include "CFileReader.h"
#include "utils.h"



ZbxSender* sender;

FileReader * fileReader;


BOOL CALLBACK CosonleHandler(DWORD ev)
{
	Sleep(1000);
	switch (ev)
	{
	case CTRL_CLOSE_EVENT:
		//case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_SHUTDOWN_EVENT:{
        std::cout << "shut down manually!" << std::endl; 
        if (fileReader) {
            sender->p_config_->SetMonitorLineNum(fileReader->GetCurrentLineNum());
            sender->p_config_->RewriteJsonToFile();
        }
        //TODO 需等待内部进程退出后 再最终执行退出  而不是只等待2s;
        _exit(-1);
    }
	default:
		break;
	}
	return true;
}

// string to wstring
void StringToWstring(std::wstring& szDst, std::string str)
{
	std::string temp = str;
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL,0); 
	wchar_t * wszUtf8 = new wchar_t[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	szDst = wszUtf8;
	std::wstring r = wszUtf8;
	delete[] wszUtf8;
}



int main() {

    //todo handle shell exception

    std::cout << "Monitor Shell started!" << std::endl;

    SetConsoleCtrlHandler(CosonleHandler, true);

    //加载zabbix 配置, 并启动zabbix sender
     sender =  new ZbxSender("monitor/zabbix.json");


    //启动程序， 设置输出的回调;

        //start cmd  /c "learning.exe 2>&1 | tee std.log"
        //the right way to capture the stdout and stderr

#ifdef STARTUP
    {
        std::cout << "startup mode" << std::endl;
        sender->p_config_->SetMonitorLineNum(0);

        char start_exe_cmd[256];
        memset(start_exe_cmd, 0, 256);

        if (1 == sender->p_config_->GetMonitorOutFlag()) {
             sprintf(start_exe_cmd, "start cmd  /c \"%s 2>&1 | tee %s\"",
                sender->p_config_->GetMonitorExe().c_str(), sender->p_config_->GetMonitorLog().c_str());
        }
        else {
             sprintf(start_exe_cmd, "start %s",
                sender->p_config_->GetMonitorExe().c_str());
        }
        exec(start_exe_cmd);
        Sleep(1000);
    }
#endif

    //启动stdout文件读取监听
    fileReader = new FileReader(sender->p_config_->GetMonitorLog().c_str(), 0, 100);

    fileReader->RegisterLineProcessFunc(std::bind(&ZbxSender::Check_and_Send, sender, std::placeholders::_1));

    fileReader->StartProcess();

    getchar();

    return 0;
}