#ifndef __COMMON_CPP__
#define __COMMON_CPP__

#include <iostream>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const std::string fifoname = "fifo";

mode_t mode = 0666;
#define SIZE 128

#endif