#include "CFileReader.h"

FileReader::FileReader(const char * filepath, int lineNum, int interval): 
        fin_(filepath, std::ios::in), 
        currentLineNum_(lineNum),
        interval_(interval)
{
 if (!fin_) {
        std::cerr << "Faile to read file" << std::endl;
        _exit(-1);
    }

    fin_.seekg(0, std::ios::beg);

    std::string line;

    // 跳转到lineNum所在行
    if (currentLineNum_ > 0) {
        int idx = 0;
        bool read_flag = false;
        while(!fin_.eof()) {
            getline(fin_, line);
            if (line.empty())
                continue;

            idx ++;
            if (idx == currentLineNum_) {
                read_flag = true;
                break;
            }
        }
    }
    std::cout << "Last Process line " << currentLineNum_ << ":" << line.c_str() << std::endl;


    thread_proc_ = new std::thread(&FileReader::Process, this);

}


void FileReader::StartProcess() {
    isRunning_ = true;
}

void FileReader::Process() {

    std::string line;

    size_t seek = fin_.tellg();

    while (isRunning_)
    {   
        // std::cout << "file size is " << seek << std::endl;

        if (!fin_.is_open()) {
            std::cerr<< "File cant open while process" << std::endl;
            continue;
        }

        if (fin_.peek() != EOF)
        {
            getline(fin_, line);
            if (!line.empty())
            {
                std::cout << "msg from stdout is : " << line.c_str() << std::endl;
                lineProcFunc_cb_(line);
                currentLineNum_++;
            }
            else
            {
                // std::cout << "line empty" << std::endl;
            }
            seek = fin_.tellg();

        }
        else {
            fin_.clear();
			fin_.seekg(seek, std::ios::beg);
			Sleep(3000);
			continue;

        }
        Sleep(interval_);
    }

    std::cout << "Process exit" << std::endl;
}


int FileReader::GetCurrentLineNum() {
    return currentLineNum_;
}


void FileReader::RegisterLineProcessFunc(std::function<void(std::string)>  func) {

    lineProcFunc_cb_ = func;
    
}