#pragma once
#include "bookingTicket.h"
#define N 255 

//生成长度为len的随机数字符串
char* creatNum(int len);

//获取时间年月日
char* getCurrentTimeStr();

//获取文件的大小
int getFileSize(FILE* fp);

//美化
void Beautify();