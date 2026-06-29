#include "NamePipe.hpp"
//这里server读
int main()
{
    //1.创建管道
    NamePipe name_pipe(fifoname);
    name_pipe.create();

    //2.打开文件-读/写,只有一端打开会阻塞
    name_pipe.OpenForRead();

    //3.通信-接受信息
    std::string message;
    while(true)
    {
        bool res = name_pipe.Read(&message);
        if(!res)
            break;
        std::cout <<"client say@ "<< message << std::endl;
    }

    //4.关闭管道
    name_pipe.Close();


    return 0;
}