#include "ProcessPool.hpp"

int main()
{
    //1.创建管道
    ProcessPool pp(5);

    //2.初始化管道
    pp.InitProcess([](int fd)
    {
        while(true)
        {
            int code=0;
            ssize_t n=read(fd,&code,sizeof(code));
            if(n==sizeof(code))
            {
                if(code>=0 && code<tasks.size())
                {
                    std::cout<<"子进程被唤醒"<<getpid()<<std::endl;
                    tasks[code]();
                }
                else{
                    std::cout<<"父进程派发的任务不对"<<getpid()<<std::endl;
                }
            }
            else if(n==0)//写端关闭，要退了
            {
                std::cout<<"写端关闭,子进程应该退了"<<getpid()<<std::endl;
                break;
            }
            else
            {
                std::cout<<"父进程给的任务码有问题"<<std::endl;
                break;
            }
        }
    });

    //3.控制管道,轮询管道，派发10个任务
    pp.PollingCtrlProcess(10);

    //4.结束，回收管道
    std::cout<<"父进程控制结束，已回收"<<std::endl;
    pp.EndProcess();

    return 0;
}