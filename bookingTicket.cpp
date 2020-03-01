#include "bookingTicket.h"
#include "util.h"
//实现候补订票所用到的全局变量
char id1[200][10], name1[200][20], airplanenum1[200][8], seat1[200], from1[200][20], to1[200][20], departure1[200][20], arrival1[200][20];
int mubiao = -1;  double prize[200];

static char o[1024 * 1024] = { '\0' };

localInfo* InitAirport(listPlaneInfo* planes,
listCusInfo* customers, listTicketInfo* tickets)
{
	//空间分配
	localInfo* airport = (localInfo*)malloc(sizeof(localInfo));
	airport->airplane = (char*)malloc(8 * sizeof(char));
	strcpy(airport->airplane, "厦门高崎");

	airport->customers = customers;
	airport->tickets = tickets;
	airport->planes = planes;

	return airport;
}

void InitAirline(listAirline& airline, localInfo* airport)
{
	FILE* fp;
	if ((fp = fopen("data\\airline.txt", "rt+")) == NULL)
	{
		exit(1);
	}

	if (getFileSize(fp) == 0)
	{
		fclose(fp);
		return;
	}

	Airline* al;
	while (!feof(fp))
	{
		al = (Airline*)malloc(sizeof(Airline));
		al->flightNumber = (char*)malloc(16 * sizeof(char));
		al->planeNumber = (char*)malloc(16 * sizeof(char));
		al->airport = (char*)malloc(16 * sizeof(char));
		al->terminus = (char*)malloc(16 * sizeof(char));
		al->takeOffTime = (char*)malloc(16 * sizeof(char));
		al->arrivalTime = (char*)malloc(16 * sizeof(char));
		al->customersMaxinum = 0;
		al->surplusTickets = 0;

		al->FPrice = 0.0;
		al->BPrice = 0.0;
		al->EPrice = 0.0;

		fscanf(fp, "%s %s %s %s %s %s %lf %lf %lf \n", al->flightNumber,
			al->planeNumber, al->airport, al->terminus, al->takeOffTime,
			al->arrivalTime, &al->FPrice, &al->BPrice, &al->EPrice);  //读取一行

		al->customersInfo = new listTicketInfo();
		GetCusList(al->customersInfo, airport->tickets, al->flightNumber);

		al->customersOfWaiting = new listTicketInfo();

		al->customersMaxinum = CusMaxOfPlane(*airport->planes, al->planeNumber);

		al->surplusTickets = al->customersMaxinum - (int)al->customersInfo->size();

		airline.push_back(al);
		al = NULL;
	}

	fclose(fp);
}

void InitPlaneInfo(listPlaneInfo& planes)
{
	FILE* fp;

	if ((fp = fopen("data\\planes.txt", "rt+")) == NULL)
	{
		exit(1);
	}

	if (getFileSize(fp) == 0)
	{
		fclose(fp);
		return;
	}

	PlaneInfo* plane;
	while (!feof(fp))
	{
		plane = (PlaneInfo*)malloc(sizeof(PlaneInfo));
		plane->FSeat = (PlaneCabin*)malloc(sizeof(PlaneCabin));
		plane->FSeat->cabinType = 'F';
		plane->BSeat = (PlaneCabin*)malloc(sizeof(PlaneCabin));
		plane->BSeat->cabinType = 'B';
		plane->ESeat = (PlaneCabin*)malloc(sizeof(PlaneCabin));
		plane->ESeat->cabinType = 'E';

		plane->planeNumber = (char*)malloc(16 * sizeof(char));

		int f, b, e;
		fscanf(fp, "%s %ld %ld %ld \n", plane->planeNumber,
			&f, &b, &e);  //读取一行

		plane->FSeat->cusMax = f;
		plane->BSeat->cusMax = b;
		plane->ESeat->cusMax = e;

		plane->customersMax = plane->FSeat->cusMax
			+ plane->BSeat->cusMax
			+ plane->ESeat->cusMax;

		plane->FSeat->seatNum = (int*)malloc(f * sizeof(int));
		plane->BSeat->seatNum = (int*)malloc(b * sizeof(int));
		plane->ESeat->seatNum = (int*)malloc(e * sizeof(int));

		planes.push_back(plane);
		plane = NULL;
	}

	fclose(fp);
}

void InitCusInfo(listCusInfo& customers)
{
	FILE* fp;

	if ((fp = fopen("data\\customers.txt", "rt+")) == NULL)
	{
		exit(1);
	}

	if (getFileSize(fp) == 0)
	{
		fclose(fp);
		return;
	}

	CusInfo* cus;
	while (!feof(fp))
	{

		cus = (CusInfo*)malloc(sizeof(CusInfo));
		cus->customerNumber = (char*)malloc(16 * sizeof(char));
		cus->name = (char*)malloc(16 * sizeof(char));
		cus->sex = (char*)malloc(16 * sizeof(char));
		cus->mobileNumber = (char*)malloc(16 * sizeof(char));
		cus->ID = (char*)malloc(32 * sizeof(char));
		cus->ticketVol = 0;

		fscanf(fp, "%s %s %s %s %s %ld \n", cus->customerNumber, cus->name, cus->sex, cus->mobileNumber, cus->ID, &cus->ticketVol);  //读取一行

		customers.push_back(cus);
		cus = NULL;
	}

	fclose(fp);
}

void InitTicketInfo(listTicketInfo& tickets)
{
	FILE* fp;

	if ((fp = fopen("data\\tickets.txt", "rt+")) == NULL)
	{
		exit(1);
	}

	if (getFileSize(fp) == 0)
	{
		fclose(fp);
		return;
	}

	TicketInfo* tic;
	while (!feof(fp))
	{
		tic = (TicketInfo*)malloc(sizeof(TicketInfo));
		tic->ticketNumber = (char*)malloc(32 * sizeof(char));
		tic->customerNumber = (char*)malloc(32 * sizeof(char));
		tic->customerName = (char*)malloc(16 * sizeof(char));
		tic->price = 0.0;
		tic->cabinType = 'E';
		tic->seat = 0;
		tic->airport = (char*)malloc(16 * sizeof(char));
		tic->terminus = (char*)malloc(16 * sizeof(char));
		tic->takeOffTime = (char*)malloc(16 * sizeof(char));
		tic->arrivalTime = (char*)malloc(16 * sizeof(char));

		fscanf(fp, "%s %s %s %lf %c %ld %s %s %s %s \n", tic->ticketNumber,
			tic->customerNumber, tic->customerName, &tic->price,
			&tic->cabinType, &tic->seat, tic->airport, tic->terminus,
			tic->takeOffTime, tic->arrivalTime);  //读取一行


		tickets.push_back(tic);
		tic = NULL;
	}

	fclose(fp);
}

int SupTiOfPlane(listTicketInfo ticketList, int max, char* alname)
{
	int ticketed = 0;
	listTicketInfo::iterator ti;
	char* ch;

	for (ti = ticketList.begin(); ti != ticketList.end(); ti++)
	{
		ch = (*ti)->ticketNumber;
		if (!strcmp(alname, ch))
		{
			ticketed++;
		}
	}

	return max - ticketed;
}

int CusMaxOfPlane(listPlaneInfo planes, char* planeName)
{
	listPlaneInfo::iterator pl;
	int max;
	char* ch;

	for (pl = planes.begin(); pl != planes.end(); pl++)
	{
		ch = (*pl)->planeNumber;
		max = (*pl)->customersMax;
		if (!strcmp(planeName, ch))
		{
			break;
		}
	}
	return max;
}

void GetCusList(listTicketInfo* cusList, listTicketInfo* ticketList, char* fn)
{
	listTicketInfo::iterator ti;
	char* ch;

	for (ti = ticketList->begin(); ti != ticketList->end(); ti++)
	{
		ch = (*ti)->ticketNumber;
		if (!strcmp(fn, ch))
		{
			cusList->push_back(*ti);
		}
	}
}

void PrintAirline(listAirline airline)
{
	char item[7][32] = { "航班 airline","客机 Num","出港 From","终点 To","起飞时间 Departure",
		"到达时间 Arrival","余票 Surplus ticket" };
	cout << "\t+-----------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
	printf("\t|%16s | %16s | %16s | %16s | %20s | %20s | %20s | \n",
		item[0], item[1], item[2], item[3], item[4], item[5], item[6]);
	listAirline::iterator al;
	string localtime = GetTime();
	for (al = airline.begin(); al != airline.end(); al++)
	{
		if (localtime<=(*al)->takeOffTime) {
			cout << "\t+-----------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
			printf("\t|%16s | %16s | %16s | %16s | %20s | %20s | %20d | \n",
				(*al)->flightNumber, (*al)->planeNumber, (*al)->airport, (*al)->terminus, (*al)->takeOffTime, (*al)->arrivalTime, (*al)->surplusTickets);
		}
	}
	cout << "\t+-----------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
}

/*****************************************************************************************/

int Service(listAirline airline, localInfo* airport)
{
	int action = 0, c = 0,flag=0;
	listAirline findAirlines;

	cout << endl << "\t请选择您需要的服务：" << endl << endl;

	cout << "\t1、航线查询\n \t2、订票业务\n \t3、退票业务\n \t4、添加机票\n \t5、订票名单\n \t-1、退出系统\n" << endl << "\t";

	scanf("%d", &action);

	if (action < -1 || action > 10)
	{
		cout << "\t 不存在此服务" << endl;
	}
	else
	{
		switch (action)
		{
		case 0:
			cout << "\t没有这个选项" << endl;
			gets_s(o);
			//return -2;
			break;
		case -1:
			if (OutService() == 1)
			{
				return 0;
			}
			else
			{
				return -1;
			}
			break;
		case 1:
			if (!SeachService(airline, &findAirlines))
			{
				break;
			}

			cout << "\t查询结果: " << endl;
			PrintAirline(findAirlines);

			cout << "\t您要订购此飞机票吗？Y/N" << endl;
			cout << "\t";
			c = Confirm();

			if (c == 1)
			{
				BookingService(findAirlines, airport);
				SaveAirline(&airline);
				SaveTicket(airport->tickets);
			}
			else if (c == 0)
			{
				break;
			}
			else
			{
				cout << "\t未知输入，退出查询。" << endl;
			}
			break;
		case 2:
			BookingService(airline, airport);
			SaveAirline(&airline);
			SaveTicket(airport->tickets);
			break;
		case 3:
			flag=RefundService(airport, &airline);
			SaveAirline(&airline);
			SaveTicket(airport->tickets);
			if (flag == 1) {
				ofstream   ofresult("data\\tickets.txt", ios::app);
				ofresult << airplanenum1[mubiao] << " " << id1[mubiao] << " " << name1[mubiao] << " " << prize[mubiao] << " " << seat1[mubiao] << " " << "0" << " " << from1[mubiao] << " " << to1[mubiao] << " " << departure1[mubiao] << " " << arrival1[mubiao] <<endl;
			}
			break;
		case 4:
			AddAirline(airline,airport);
			break;
		case 5:
			Outputticket();
			break;
		default:
			cout << "\t此服务未开发" << endl;
			break;
		}
	}

	cout << "\t";
	system("pause");
	return -1;
}

/*****************************************************************************************/

int OutService()
{
	char ch = 'Y';

	while (true)
	{
		cout << "\t您要确认退出吗？ Y/N" << endl;
		cout << "\t";

		int c = Confirm();

		if (c == 1)
		{
			cout << "\t正在退出..." << endl;
			return 1;
		}
		else if (c == 0)
		{
			return 0;
		}
		else
		{
			cout << "\t未知输入，请重新输入！" << endl;
		}
	}
}

int SeachService(listAirline airline, listAirline* a)
{
	char from[16], to[16];

	while (true)
	{
		cout << "\t请输入起点站：" << endl << "\t";
		cin >> from;
		cout << "\t请输入终点站：" << endl << "\t";
		cin >> to;

		if (!IsAirport(airline, from) || !IsAirport(airline, to))
		{
			cout << "\t不存在此线路" << endl;
			cout << "\t";
			system("pause");

			cout << "\t是否继续查找？Y/N" << endl;
			cout << "\t";

			int c = Confirm();
			if (c == 1)
			{
				continue;
			}
			else if (c == 0)
			{
				return 0;
			}
			else
			{
				cout << "\t未知输入！" << endl;
				return 0;
			}
		}
		else
		{
			break;
		}
	}

	SeachByFT(airline, from, to, a);

	return 1;
}

int Confirm()
{
	char ch;
	cin >> ch;
	gets_s(o);

	if (ch == 'y' || ch == 'Y')
	{
		return 1;
	}
	else if (ch == 'n' || ch == 'N')
	{
		return 0;
	}
	else
	{
		return -1;
	}

	return 1;
}

int IsAirport(listAirline airlines, char* airport)
{
	listAirline::iterator al;
	for (al = airlines.begin(); al != airlines.end(); al++)
	{
		if (!strcmp((*al)->terminus, airport) || !strcmp((*al)->airport, airport))
		{
			return 1;
		}
	}

	return 0;
}

void SeachByFT(listAirline airlines, char* from, char* to, listAirline* als)
{

	listAirline::iterator al;
	for (al = airlines.begin(); al != airlines.end(); al++)
	{
		if (!strcmp((*al)->terminus, to) && !strcmp((*al)->airport, from))
		{
			(*als).push_back(*al);
		}
	}
}

int BookingService(listAirline airline, localInfo* airport)
{
	char al[16], id[32];
	int c = 1;

start:  system("cls");
	cout << endl << "\t\t\t\t\t----------" << "订票 Booking" << "----------" << endl << endl;

	PrintAirline(airline);

	cout << "\t请输入您要预定的航班号: " << endl;
	cout << "\t";

	cin >> al;

	if (IsArilineNum(airline, al) == 1)
	{
		//用户验证（文件）
		cout << "\t请输入您的证件号:";
		cin >> id;

		if (!CheckCustomer(airport, id))
		{
			//不存在，创建用户，保存用户
			CusInfo* nC = new CusInfo();
			nC->customerNumber = (char*)malloc(16 * sizeof(char));
			nC->name = (char*)malloc(16 * sizeof(char));
			nC->sex = (char*)malloc(16 * sizeof(char));
			nC->mobileNumber = (char*)malloc(16 * sizeof(char));
			nC->ID = (char*)malloc(32 * sizeof(char));
			nC->ticketVol = 0;

			while (true)
			{
				if (strcmp(id, "0") != 0)
				{
					strcpy(nC->ID, id);
				}
				else
				{
					cout << "\t请输入证件号：" << endl;
					cin >> nC->ID;
				}

				cout << "\t请输入姓名：";
				cin >> nC->name;

				cout << "\t请输入姓别：";
				cin >> nC->sex;

				cout << "\t请输入手机号：";
				cin >> nC->mobileNumber;

				cout << "\t确认信息？Y/N" << endl;

				cout << "\t客户证件号：" << nC->ID << " 姓名：" << nC->name << " " << nC->sex << " 手机号：" << nC->mobileNumber << endl << "\t";

				char* cusNum = strcat(getCurrentTimeStr(), creatNum(3));
				strcpy(nC->customerNumber, cusNum);

				int ccc = Confirm();
				if (ccc == 1)
				{
					(*airport->customers).push_back(nC);
					SaveCustomer(airport->customers);
					break;
				}
				else if (ccc == 0)
				{
					strcpy(id, "0");
					continue;
				}
				else
				{
					cout << "\t未知输入,默认否定" << endl;
					strcpy(id, "0");
					continue;
				}
			}
		}
		//订票操作
		Booking(airport, &airline, id, al);
	}
	else
	{
		cout << "\t不存在该航班\n\t是否要继续？Y/N" << endl;
		cout << "\t";
		c = Confirm();
		if (c == 1)
		{
			goto start;
		}
		else
		{
			cout << "\t退出购票" << endl;
		}
	}

	return 0;
}

int IsArilineNum(listAirline airlines, char* num)
{
	listAirline::iterator al;
	for (al = airlines.begin(); al != airlines.end(); al++)
	{
		if (!strcmp((*al)->flightNumber, num))
		{
			return 1;
		}
	}

	return 0;
}

int CheckCustomer(localInfo* airport, char* ID)
{
	listCusInfo u = *airport->customers;

	listCusInfo::iterator c;
	for (c = u.begin(); c != u.end(); c++)
	{
		if (!strcmp((*c)->ID, ID))
		{
			cout << "\t存在客户：" << (*c)->name << " " << (*c)->sex << " 手机号：" << (*c)->mobileNumber << endl;
			cout << "\t前往订票" << endl;

			return 1;
		}
	}
	return 0;

}

void Booking(localInfo* airport, listAirline* airline, char* id, char* num)
{
	listTicketInfo* tis = airport->tickets;
	listCusInfo* cus = airport->customers;
	CusInfo* cc = NULL;
	Airline* aa = NULL;
	TicketInfo* ti = new TicketInfo();

	ti->ticketNumber = (char*)malloc(32 * sizeof(char));
	ti->customerNumber = (char*)malloc(32 * sizeof(char));
	ti->customerName = (char*)malloc(16 * sizeof(char));
	ti->price = 0.0;
	ti->cabinType = 'E';
	ti->seat = 0;
	ti->airport = (char*)malloc(16 * sizeof(char));
	ti->terminus = (char*)malloc(16 * sizeof(char));
	ti->takeOffTime = (char*)malloc(16 * sizeof(char));
	ti->arrivalTime = (char*)malloc(16 * sizeof(char));

	cc = getCusInfoById(cus, id);
	strcpy(ti->ticketNumber, num);
	strcpy(ti->customerNumber, cc->ID);
	strcpy(ti->customerName, cc->name);

	aa = getAirlineByNum(airline, num);
	ti->seat = aa->surplusTickets;
intype:
	if (aa->surplusTickets == 0) {
		cout << endl << endl;
		cout << "\t对不起 , 票已售完!! " << endl;
		cout << "\t预定购票请输入(Y):";
		int cc = Confirm();
		char airplane[20];
		if (cc) {
			char jicang;
			cout << "\t请输入所要预定的机舱:";
			cin >> jicang;
			while (jicang != 'F' && jicang != 'E' && jicang != 'B') {
				cout << "输入错误请重新输入" << endl;
				cin >> jicang;
			}
			ofstream ofresult("data\\nexttickets.txt", ios::app);
			ofresult << ti->customerNumber << " " << ti->customerName << " " << ti->ticketNumber << " " << aa->airport << " " << aa->terminus << " " << aa->takeOffTime << " " << aa->arrivalTime << " " << jicang << aa->FPrice << endl;
			cout << "\t候补订票成功" << endl;
		}
		return;
	}
	cout << "\t请选择您需要订的机舱（F,B,E)" << endl << "\t";
	cin >> ti->cabinType;
	if (ti->cabinType == 'F' || ti->cabinType == 'f')
	{
		ti->cabinType = 'F';
		ti->price = aa->FPrice;
	}
	else if (ti->cabinType == 'B' || ti->cabinType == 'b')
	{
		ti->cabinType = 'B';
		ti->price = aa->BPrice;
	}
	else if (ti->cabinType == 'E' || ti->cabinType == 'e')
	{
		ti->cabinType = 'E';
		ti->price = aa->EPrice;
	}
	else
	{
		cout << "\t输入错误，不存在机舱\n";
		goto intype;
	}
	strcpy(ti->airport, aa->airport);
	strcpy(ti->terminus, aa->terminus);
	strcpy(ti->takeOffTime, aa->takeOffTime);
	strcpy(ti->arrivalTime, aa->arrivalTime);

	(*tis).push_back(ti);
	aa->surplusTickets--;
	aa->customersInfo->push_back(ti);

	cout << "\t预订成功！" << endl;
	cout << "\t机票信息：" << endl << "\t航班号：" << ti->ticketNumber << " 客户证件号： " << ti->customerNumber << " 客户姓名： " << ti->customerName
		<< " 价格： " << ti->price << " 舱位： " << ti->cabinType << endl << "\t起点： " << ti->airport << " 终点： " << ti->terminus
		<< " 起飞时间： " << ti->takeOffTime << " 降落时间： " << ti->arrivalTime << endl;
}

CusInfo* getCusInfoById(listCusInfo* cus, char* id)
{
	listCusInfo::iterator c;
	CusInfo* cc = NULL;

	for (c = (*cus).begin(); c != (*cus).end(); c++)
	{
		if (strcmp((*c)->ID, id) == 0)
		{
			cc = *c;
			break;
		}
	}
	return cc;
}

Airline* getAirlineByNum(listAirline* als, char* num)
{
	listAirline::iterator a;
	Airline* aa = NULL;

	for (a = (*als).begin(); a != (*als).end(); a++)
	{
		if (strcmp((*a)->flightNumber, num) == 0)
		{
			aa = *a;
			break;
		}
	}
	return aa;
}

listTicketInfo* getTicketInfoById(listTicketInfo* tkl, char* id)
{
	listTicketInfo::iterator tt;
	listTicketInfo* aa = new listTicketInfo();

	for (tt = (*tkl).begin(); tt != (*tkl).end(); tt++)
	{
		if (strcmp((*tt)->customerNumber, id) == 0)
		{
			aa->push_back(*tt);
		}
	}
	return aa;
}

TicketInfo* getTicketInfoByNum(listTicketInfo* tkl, char* num)
{
	listTicketInfo::iterator tt;
	TicketInfo* t = NULL;

	for (tt = (*tkl).begin(); tt != (*tkl).end(); tt++)
	{
		if (strcmp((*tt)->ticketNumber, num) == 0)
		{
			t = *tt;
			break;
		}
	}
	return t;
}

int RefundService(localInfo* airport, listAirline* airline)
{
	char id[32], num[16];
	CusInfo* c = NULL;
	listTicketInfo* t = NULL;
	TicketInfo* tt = NULL;

	cout << "\t请输入您的证件号: ";
	cin >> id;

	c = getCusInfoById(airport->customers, id);
	if (c == NULL)
	{
		cout << "\t不存在证件号信息" << endl;
		cout << "\t";
		system("pause");
		return 0;
	}

	cout << "\t您的证件信息：" << c->ID << "  " << c->name << " " << c->sex << " 手机号： " << c->mobileNumber << endl;

	t = getTicketInfoById(airport->tickets, c->ID);
	if (t->size() == 0)
	{
		cout << "\t不存在机票信息" << endl;
		cout << "\t";
		system("pause");
		return 0;
	}

	cout << endl << "\t机票信息：" << endl << endl;
	listTicketInfo::iterator ti;
	for (ti = t->begin(); ti != t->end(); ti++)
	{
		cout << "\t航班号：" << (*ti)->ticketNumber << endl << "\t客户证件号： " << (*ti)->customerNumber << " 客户姓名： " << (*ti)->customerName
			<< " 价格： " << (*ti)->price << " 舱位： " << (*ti)->cabinType << endl << "\t起点： " << (*ti)->airport << " 终点： " << (*ti)->terminus
			<< " 起飞时间： " << (*ti)->takeOffTime << " 降落时间： " << (*ti)->arrivalTime << endl << endl;
	}

	cout << "\t请输入您要退票的航班号: ";
	cin >> num;

	tt = getTicketInfoByNum(t, num);
	if (tt == NULL)
	{
		cout << "\t您不存在该航班机票" << endl;
		cout << "\t";
		system("pause");
		return 0;
	}

	cout << "\t退票信息：" << endl;
	cout << "\t航班号：" << tt->ticketNumber << endl << "\t客户证件号： " << tt->customerNumber << " 客户姓名： " << tt->customerName
		<< " 价格： " << tt->price << " 舱位： " << tt->cabinType << endl << "\t起点： " << tt->airport << " 终点： " << tt->terminus
		<< " 起飞时间： " << tt->takeOffTime << " 降落时间： " << tt->arrivalTime << endl << endl;

	cout << "\t确认要退掉此票吗？Y/N" << endl << "\t";
	int cc = Confirm();
	if (cc == 1)
	{
		//退票
		Refund(airport, airline, tt);
		cout << "\t是否开启候补订票检查（Y/N）： ";
		int flag= Confirm();
		if (flag == 1) {
			cout << "\n\n\t\t候补订票检查:" << endl;
			printf("\t");
			for (int i = 0; i <= 30; i++) {
				printf("*");
				Sleep(20);
			}
			if (Nexttickets(num) == 1) {
				return 1;
			}
			else {
				return 0;
			}
			system("pause");
		}
	}
	else if (cc == 0)
	{
		//不退票
		cout << "\t好的，不退票" << endl;
		cout << "\t";
		system("pause");
	}
	else
	{
		cout << "\t未知输入，不退票" << endl;
		cout << "\t";
		system("pause");
	}
}

void Refund(localInfo* airport, listAirline* airline, TicketInfo* ticket)
{
	listTicketInfo* inti = NULL;
	listAirline::iterator la;
	listTicketInfo::iterator lt;

	for (la = airline->begin(); la != airline->end(); la++)
	{
		if (strcmp((*la)->flightNumber, ticket->ticketNumber) == 0)
		{
			inti = (*la)->customersInfo;
			break;
		}
	}

	listTicketInfo::iterator intt;
	intt = inti->begin();
	while (intt != inti->end())
	{
		if (strcmp((*intt)->ticketNumber, ticket->ticketNumber) == 0)
		{
			intt = inti->erase(intt);
			continue;
		}
		if (inti->size() != 0)
		{
			intt++;
		}
	}

	lt = airport->tickets->begin();
	while (lt != airport->tickets->end())
	{
		if (strcmp((*lt)->ticketNumber, ticket->ticketNumber) == 0)
		{
			lt = airport->tickets->erase(lt);
			continue;
		}
		if (airport->tickets->size() != 0)
		{
			lt++;
		}
	}
}

void SaveCustomer(listCusInfo* customers)
{
	FILE* fp;

	if ((fp = fopen("data\\customers_temp.txt", "wt+")) == NULL)
	{
		exit(1);
	}

	listCusInfo::iterator cu;
	for (cu = customers->begin(); cu != customers->end(); cu++)
	{
		fprintf(fp, "%s %s %s %s %s %ld \n",
			(*cu)->customerNumber, (*cu)->name, (*cu)->sex,
			(*cu)->mobileNumber, (*cu)->ID, (*cu)->ticketVol);
	}
	fclose(fp);

	remove("data\\customers.txt");
	rename("data\\customers_temp.txt", "data\\customers.txt");
	remove("data\\customers_temp.txt");
}

void SaveTicket(listTicketInfo* tickets)
{
	FILE* fp;
	if ((fp = fopen("data\\tickets_temp.txt", "wt+")) == NULL)
	{
		exit(1);
	}

	listTicketInfo::iterator tic;
	for (tic = tickets->begin(); tic != tickets->end(); tic++)
	{
		fprintf(fp, "%s %s %s %lf %c %ld %s %s %s %s \n", (*tic)->ticketNumber,
			(*tic)->customerNumber, (*tic)->customerName, (*tic)->price,
			(*tic)->cabinType, (*tic)->seat, (*tic)->airport, (*tic)->terminus,
			(*tic)->takeOffTime, (*tic)->arrivalTime);
	}

	fclose(fp);

	remove("data\\tickets.txt");
	rename("data\\tickets_temp.txt", "data\\tickets.txt");
	remove("data\\tickets_temp.txt");
}

void SaveAirline(listAirline* airlines)
{
	FILE* fp;

	if ((fp = fopen("data\\airline_temp.txt", "wt+")) == NULL)
	{
		exit(1);
	}

	listAirline::iterator al;
	for (al = airlines->begin(); al != airlines->end(); al++)
	{
		fprintf(fp, "%s %s %s %s %s %s %lf %lf %lf \n", (*al)->flightNumber,
			(*al)->planeNumber, (*al)->airport, (*al)->terminus, (*al)->takeOffTime,
			(*al)->arrivalTime, (*al)->FPrice, (*al)->BPrice, (*al)->EPrice);
	}

	fclose(fp);

	remove("data\\airline.txt");
	rename("data\\airline_temp.txt", "data\\airline.txt");
	remove("data\\airline_temp.txt");
}

void AddAirline(listAirline& airline, localInfo* airport)
{
	Airline* al;
	cout << "\t确定添加信息请输入(Y): ";
	int cc = Confirm();  //用于判断是否添加航班信息
	while (cc)
	{
		ofstream ofresult1("data\\airline.txt", ios::app);
		al = (Airline*)malloc(sizeof(Airline));
		al->flightNumber = (char*)malloc(16 * sizeof(char));
		al->planeNumber = (char*)malloc(16 * sizeof(char));
		al->airport = (char*)malloc(16 * sizeof(char));
		al->terminus = (char*)malloc(16 * sizeof(char));
		al->takeOffTime = (char*)malloc(16 * sizeof(char));
		al->arrivalTime = (char*)malloc(16 * sizeof(char));
		al->FPrice = 0.0;
		al->BPrice = 0.0;
		al->EPrice = 0.0;
		int Fseat, Eseat, Bseat;
		cout << endl;
		cout << "\t请输入航班号：";
		cin >> al->flightNumber;

		cout << "\t请输入飞机型号：";
		cin >> al->planeNumber;
		cout << "\t起点:";
		cin >> al->airport;
		cout << "\t终点:";
		cin >> al->terminus;
		cout << "\t起飞时间:" << "(输入格式例:2019-07-08+22:00）";
		cin >> al->takeOffTime;
		cout << "\t到达时间:" << "(输入格式例:2019-07-08+22:00) ";
		cin >> al->arrivalTime;
		cout << "\t请分别输入三种票价:  (及座位额度)" << endl;;
		cout << "\t头等舱(F):";
		cin >> al->FPrice;   cout << "\t座位额度:";   cin >> Fseat;
		cout << "\t商务舱(B):";
		cin >> al->BPrice;   cout << "\t座位额度:";   cin >> Bseat;
		cout << "\t经济舱(E):";
		cin >> al->EPrice;   cout << "\t座位额度:";   cin >> Eseat;
		ofresult1 << al->flightNumber << " " << al->planeNumber << " " << al->airport << " " << al->terminus<<" ";
		ofresult1 << al->takeOffTime << " " << al->arrivalTime << " " << al->FPrice << " " << al->BPrice << " " << al->EPrice <<" "<< endl;
		ofstream ofresult2("data\\planes.txt", ios::app);
		ofresult2 << al->planeNumber << " " << Fseat << " " << Bseat << " " << Eseat;
		cout << "\t继续请输入(Y): ";
		cc = Confirm();
	}
}

void Outputticket()
{
	FILE* fp;
	if ((fp = fopen("data\\tickets.txt", "r")) == NULL)
	{
		exit(1);
	}
	char item[10][32] = { "航班 airline","证件号 ID","姓名 Name","票价 Price","舱位 Level",
		"座位号 Seat","出港 Form","终点 To","起飞时间 Departure","到达时间 Arrival" };
	cout << "\t+------------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
	printf("\t|%10s|%10s|%13s|%12s|%10s|%10s|%16s|%16s|%17s|%17s|\n",
		item[0], item[1], item[2], item[3], item[4], item[5], item[6], item[7], item[8], item[9]);
	cout << "\t+------------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
	char airline[8], id[10], name[10], level, form[15], to[15], departure[20], arrival[20];
	int seat;   double price;
	while (!feof(fp)) {
		fscanf(fp, "%s %s %s %lf %c %d %s %s %s %s", airline, id, name, &price, &level, &seat, form, to, departure, arrival);
		if (feof(fp)) {
			break;
		}
		printf("\t|%12s|%10s|%13s|%12lf|%10c|%11d|%16s|%16s|%18s|%17s|\n", airline, id, name, price, level, seat, form, to, departure, arrival);
		cout << "\t+------------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
	}
	fclose(fp);
}


int Nexttickets(char ticketnumber[])
{
	cout << "\n\t  (每次最多搜索前两百个)" << endl;

	FILE* fp;
	if ((fp = fopen("data\\nexttickets.txt", "r")) == NULL)
	{
		exit(1);
	}
	int cnt=0,i=0;
	while (!feof(fp)) {
		fscanf(fp, "%s %s %s %s %s %s %s %c %lf", id1[i], name1[i], airplanenum1[i], from1[i], to1[i], departure1[i], arrival1[i], & seat1[i], &prize[i]);
		if (strcmp(airplanenum1[i], ticketnumber)==0) {
			mubiao = i;
		}
		i++;
	}
	cnt = i;
	fclose(fp);
	if (mubiao != -1) {
		cout << "\t按顺序查找到现可订票的候补订票人员:" << endl;
		cout << "\t+-------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
		char item[8][20] = { "证件号 ID","姓名 Name","航班号 Airplanenum","出港 From","终点 To","起飞时间 Departure","到达时间 Arrival","舱位 Level" };
		printf("\t|%14s|%14s|%18s|%18s|%18s|%18s|%18s|%14s|\n",item[0], item[1], item[2], item[3], item[4], item[5], item[6], item[7]);
		cout << "\t+-------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
		printf("\t|%14s|%14s|%18s|%18s|%18s|%18s|%18s|%14c|\n",id1[mubiao], name1[mubiao], airplanenum1[mubiao] , from1[mubiao], to1[mubiao], departure1[mubiao], arrival1[mubiao] ,seat1[mubiao]);
		cout << "\t+-------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
		cout << "\t是否为这位客户订票？Y/N:";
		int c = Confirm();
		if (c == 1) {
			if ((fp = fopen("data\\nexttickets.txt", "w+")) == NULL)
			{
				exit(1);
			}
			for (i = 0; i < cnt-1; i++) {
				if (i != mubiao) {
					fprintf(fp, "%s %s %s %s %s %s %s %c %lf\n", id1[i], name1[i], airplanenum1[i], from1[i], to1[i], departure1[i], arrival1[i], seat1[i],prize[i]);
				}
			}
			fclose(fp);
			return 1;
		}

		return 0;
	}
	else {
		cout << "\t未查找到现可订票的候补订票人员" << endl;
		return 0;
	}

}

string GetTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d+%H:%M", localtime(&timep));
	return tmp;
}