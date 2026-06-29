#include "NamePipe.hpp"
//这里client写
int main()
{ 
    NamePipe name_pipe(fifoname);
    //2.打开文件-读/写,只有一端打开会阻塞
    name_pipe.OpenForWrite();

    //3.通信-发送信息
    while(true)
    {
        std::cout<<"please enter#"<<std::endl;
        std::string line;
        std::getline(std::cin,line);
        name_pipe.Write(line);
    }

    //4.关闭管道
    name_pipe.Close();

    return 0;
}