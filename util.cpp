#include "util.h"

char* creatNum(int len)
{
	char* str = (char*)malloc(sizeof(char));
	int i;
	srand((unsigned)time(NULL));

	for (i = 0; i < len; i++)
	{
		str[i] = '0' + rand() % 10;
	}

	str[len] = '\0';

	return str;
}

char* getCurrentTimeStr()
{
	time_t t = time(NULL);
	char ch[32];

	strftime(ch, sizeof(ch) - 1, "%Y%m%d", localtime(&t));     //年-月-日 时-分-秒

	return ch;
}

int getFileSize(FILE* fp)
{
	int size = 0;

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	return size;
}

void Beautify()
{
	HANDLE handle_out;  //定义一个句柄  
	CONSOLE_SCREEN_BUFFER_INFO scbi;    //定义一个窗口缓冲区信息结构体  
	COORD size = { 16, 5 };      //定义一个坐标结构体  80 25

	//WCHAR strtitle[N];

	handle_out = GetStdHandle(STD_OUTPUT_HANDLE);   //获得标准输出设备句柄  
	GetConsoleScreenBufferInfo(handle_out, &scbi);  //获得窗口缓冲区信息  

	WCHAR title[32] = { '\0' };
	wcscpy_s(title, L"携程旅行飞机票");
	SetConsoleTitle(title);

	SetConsoleScreenBufferSize(handle_out, size);   // 重新设置缓冲区大小  

	SMALL_RECT rc = { 0, 0, 160, 48 };     // 重置窗口位置和大小  
	SetConsoleWindowInfo(handle_out, 1, &rc);
	CloseHandle(handle_out);    //关闭标准输出设备句柄  
}