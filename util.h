#pragma once
#include "bookingTicket.h"
#define N 255 

//���ɳ���Ϊlen��������ַ���
char* creatNum(int len);

//��ȡʱ��������
char* getCurrentTimeStr();

//��ȡ�ļ��Ĵ�С
int getFileSize(FILE* fp);

//����
void Beautify();