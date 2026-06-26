#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

using Task_t=std::function<void()>;

void DownLoad()
{
    std::cout<<"我是一个DownLoad任务"<<std::endl;
}
void MySQL()
{
    std::cout<<"我是一个MySQL任务"<<std::endl;
}
void Sycn()
{
    std::cout<<"我是一个Sycn任务"<<std::endl;
}
void Log()
{
    std::cout<<"我是一个Log任务"<<std::endl;
}

std::vector<Task_t> tasks;
class Init
{
public:
    Init()
    {
        tasks.push_back(DownLoad);
        tasks.push_back(MySQL);
        tasks.push_back(Sycn);
        tasks.push_back(Log);
    }
};

Init ginit;