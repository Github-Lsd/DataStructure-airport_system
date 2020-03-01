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

	strftime(ch, sizeof(ch) - 1, "%Y%m%d", localtime(&t));     //��-��-�� ʱ-��-��

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
	HANDLE handle_out;  //����һ�����  
	CONSOLE_SCREEN_BUFFER_INFO scbi;    //����һ�����ڻ�������Ϣ�ṹ��  
	COORD size = { 16, 5 };      //����һ������ṹ��  80 25

	//WCHAR strtitle[N];

	handle_out = GetStdHandle(STD_OUTPUT_HANDLE);   //��ñ�׼����豸���  
	GetConsoleScreenBufferInfo(handle_out, &scbi);  //��ô��ڻ�������Ϣ  

	WCHAR title[32] = { '\0' };
	wcscpy_s(title, L"Я�����зɻ�Ʊ");
	SetConsoleTitle(title);

	SetConsoleScreenBufferSize(handle_out, size);   // �������û�������С  

	SMALL_RECT rc = { 0, 0, 160, 48 };     // ���ô���λ�úʹ�С  
	SetConsoleWindowInfo(handle_out, 1, &rc);
	CloseHandle(handle_out);    //�رձ�׼����豸���  
}