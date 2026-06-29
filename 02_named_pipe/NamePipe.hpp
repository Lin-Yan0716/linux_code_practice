#pragma once
#include "Common.cpp"

const int defaultfd=-1;

class NamePipe
{
public:
    NamePipe(const std::string& name):_pipename(name)
    {}
    bool create()
    {
        int n=mkfifo(fifoname.c_str(),mode);
        if(n==0)
        {
            std::cout<<"命名管道创建成功"<<std::endl;
        }
        if(n<0)
        {
            perror("mkfifo");
            return false;
        }
        return true;
    }
    void Close()
    {
        if(_fd==defaultfd)
            return;
        else    
            close(_fd);
    }
    bool OpenForRead()
    {
        _fd=open(fifoname.c_str(),O_RDONLY);
        if(_fd<0)
        {
            perror("open");
            std::cout<<"文件打开失败"<<std::endl;
            return false;
        }
        std::cout<<"文件以读方式打开"<<std::endl;
        return true;
    }
    bool OpenForWrite()
    {
        _fd=open(fifoname.c_str(),O_WRONLY);
        if(_fd<0)
        {
            perror("open");
            std::cout<<"文件打开失败"<<std::endl;
            return false;
        }
        return true;
    }
    //从管道里读到自己的buf
    bool Read(std::string *out)
    {
        char buffer[SIZE]={0};
        ssize_t n=read(_fd,buffer,sizeof(buffer)-1);
        if(n>0)
        {
            buffer[n] = 0;
            *out = buffer;
        }
        else if(n==0)
        {
            //perror("read");
            return false;
        }
        else{
            return false;
        }
        return true;
    }
    void Write(const std::string& in)
    {
        write(_fd,in.c_str(),in.size());
    }

    ~NamePipe()
    {}
private:
    std::string _pipename;
    int _fd;
};