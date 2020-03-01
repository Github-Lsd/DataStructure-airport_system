#include "bookingTicket.h"
#include "util.h"


int main()
{
	int action = 1;

	system("title 携程飞机票");//设置cmd窗口标题
	system("mode con cols=160 lines=48");

	while (action)
	{
		//初始化
		listPlaneInfo planes;
		InitPlaneInfo(planes);

		listCusInfo customers;
		InitCusInfo(customers);

		listTicketInfo tickets;
		InitTicketInfo(tickets);

		localInfo* airport;
		airport = InitAirport(&planes, &customers, &tickets);

		listAirline airline;
		InitAirline(airline, airport);

		time_t rawtime;
		struct tm* timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		system("CLS");
		cout << endl << "\t\t\t\t\t----------" << "欢迎使用" << airport->airplane << "机场票务系统！" << "----------" << endl << endl;
		cout << "\t北京时间: " << asctime(timeinfo) << endl;

		PrintAirline(airline);       //输出航班

		switch (Service(airline, airport))
		{
		case 0:
			action = 0;
			break;
		case 1:
			break;
		case -1:
			continue;
			break;
		case -2:
			cout << "\t发生错误" << endl;
			exit(1);
			break;
		default://-1
			continue;
		}
	}

	return 0;
}