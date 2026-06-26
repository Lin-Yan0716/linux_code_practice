#ifndef __Process_Pool_Hpp__
#define __Process_Pool_Hpp__

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <functional>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "Task.hpp"

const int gdefault_num = 5;

using callback_t = std::function<void(int fd)>;

class channel
{
public:
    channel()
    {
    }
    channel(int fd, std::string &name, pid_t id) : _wfd(fd), _name(name), _sub_target(id)
    {
    }
    ~channel()
    {
    }

    void Close() { close(_wfd); }
    void Wait()
    {
        pid_t rid = waitpid(_sub_target, nullptr, 0);
        (void)rid;
    }
    int getwfd(){return _wfd;}
    std::string getname(){return _name;}
    pid_t getsubtarget(){return _sub_target;}

private:
    int _wfd;
    std::string _name;
    pid_t _sub_target; // 目标子进程
};

class ProcessPool
{
private:
    void CtrlTask(int& index)
    {
        //选择一个通道,轮询式
        int who=index;
        index++;
        index%=_channels.size();

        //随机数选择一个任务
        int chose=rand()%tasks.size();

        //把任务推送给子进程(也就是往对应管道里面写数字，表示tasks[i]里面的i值)
        std::cout << "选择信道：" << _channels[who].getname() << ",subtarget:" << _channels[who].getsubtarget() << std::endl;
        ssize_t x=write(_channels[index].getwfd(),&chose,sizeof(chose));
        sleep(2);
    }

public:
    ProcessPool(int num=gdefault_num):_processnum(num)
    {
        srand(time(nullptr) ^ getpid() ^ 0x77);
    }
    ~ProcessPool()
    {
    }
    // 1.初始化进程池
    bool InitProcess(callback_t cb)
    {
        for (int i = 0; i < gdefault_num; i++)
        {
            // 创建管道
            int pipefd[2] = {0};
            int n = pipe(pipefd);
            if (n < 0)
                return false;

            // 创建子进程
            pid_t id = fork();
            if (id < 0)
                return false;

            // 子进程，读
            if (id == 0)
            {
                // 关闭不需要的管道
                close(pipefd[1]);

                // 子进程完成自己的任务
                cb(pipefd[0]);

                // 子进程退出
                exit(0);
            }
            // 父进程，写,并将每次创建的管道放进_channels里面
            close(pipefd[0]);

            std::string name="channels-"+std::to_string(i);
            _channels.emplace_back(pipefd[1],name,id);
        }
        return true;
    }
    // 2.唤醒特定的子进程，让他们完成自己的任务
    // 轮询式
    void PollingCtrlProcess()
    {
        int index = 0;
        while (true)
        {
            CtrlTask(index);
        }
    }
    void PollingCtrlProcess(int count)
    {
        int index = 0;
        if(count<=0)return;
        while (count--)
        {
            CtrlTask(index);
        }
    }
    // 3.结束进程，让父进程回收子进程，关闭对应管道
    void EndProcess()
    {
        for (int i = _channels.size() - 1; i >= 0; i--)
        {
            _channels[i].Close();
            _channels[i].Wait();
        }
    }

private:
    int _processnum;
    std::vector<channel> _channels;
};

#endif