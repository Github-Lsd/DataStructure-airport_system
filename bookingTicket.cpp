#include "bookingTicket.h"
#include "util.h"
//ʵ�ֺ򲹶�Ʊ���õ���ȫ�ֱ���
char id1[200][10], name1[200][20], airplanenum1[200][8], seat1[200], from1[200][20], to1[200][20], departure1[200][20], arrival1[200][20];
int mubiao = -1;  double prize[200];

static char o[1024 * 1024] = { '\0' };

localInfo* InitAirport(listPlaneInfo* planes,
listCusInfo* customers, listTicketInfo* tickets)
{
	//�ռ����
	localInfo* airport = (localInfo*)malloc(sizeof(localInfo));
	airport->airplane = (char*)malloc(8 * sizeof(char));
	strcpy(airport->airplane, "���Ÿ���");

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
			al->arrivalTime, &al->FPrice, &al->BPrice, &al->EPrice);  //��ȡһ��

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
			&f, &b, &e);  //��ȡһ��

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

		fscanf(fp, "%s %s %s %s %s %ld \n", cus->customerNumber, cus->name, cus->sex, cus->mobileNumber, cus->ID, &cus->ticketVol);  //��ȡһ��

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
			tic->takeOffTime, tic->arrivalTime);  //��ȡһ��


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
	char item[7][32] = { "���� airline","�ͻ� Num","���� From","�յ� To","���ʱ�� Departure",
		"����ʱ�� Arrival","��Ʊ Surplus ticket" };
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

	cout << endl << "\t��ѡ������Ҫ�ķ���" << endl << endl;

	cout << "\t1�����߲�ѯ\n \t2����Ʊҵ��\n \t3����Ʊҵ��\n \t4����ӻ�Ʊ\n \t5����Ʊ����\n \t-1���˳�ϵͳ\n" << endl << "\t";

	scanf("%d", &action);

	if (action < -1 || action > 10)
	{
		cout << "\t �����ڴ˷���" << endl;
	}
	else
	{
		switch (action)
		{
		case 0:
			cout << "\tû�����ѡ��" << endl;
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

			cout << "\t��ѯ���: " << endl;
			PrintAirline(findAirlines);

			cout << "\t��Ҫ�����˷ɻ�Ʊ��Y/N" << endl;
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
				cout << "\tδ֪���룬�˳���ѯ��" << endl;
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
			cout << "\t�˷���δ����" << endl;
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
		cout << "\t��Ҫȷ���˳��� Y/N" << endl;
		cout << "\t";

		int c = Confirm();

		if (c == 1)
		{
			cout << "\t�����˳�..." << endl;
			return 1;
		}
		else if (c == 0)
		{
			return 0;
		}
		else
		{
			cout << "\tδ֪���룬���������룡" << endl;
		}
	}
}

int SeachService(listAirline airline, listAirline* a)
{
	char from[16], to[16];

	while (true)
	{
		cout << "\t���������վ��" << endl << "\t";
		cin >> from;
		cout << "\t�������յ�վ��" << endl << "\t";
		cin >> to;

		if (!IsAirport(airline, from) || !IsAirport(airline, to))
		{
			cout << "\t�����ڴ���·" << endl;
			cout << "\t";
			system("pause");

			cout << "\t�Ƿ�������ң�Y/N" << endl;
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
				cout << "\tδ֪���룡" << endl;
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
	cout << endl << "\t\t\t\t\t----------" << "��Ʊ Booking" << "----------" << endl << endl;

	PrintAirline(airline);

	cout << "\t��������ҪԤ���ĺ����: " << endl;
	cout << "\t";

	cin >> al;

	if (IsArilineNum(airline, al) == 1)
	{
		//�û���֤���ļ���
		cout << "\t����������֤����:";
		cin >> id;

		if (!CheckCustomer(airport, id))
		{
			//�����ڣ������û��������û�
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
					cout << "\t������֤���ţ�" << endl;
					cin >> nC->ID;
				}

				cout << "\t������������";
				cin >> nC->name;

				cout << "\t�������ձ�";
				cin >> nC->sex;

				cout << "\t�������ֻ��ţ�";
				cin >> nC->mobileNumber;

				cout << "\tȷ����Ϣ��Y/N" << endl;

				cout << "\t�ͻ�֤���ţ�" << nC->ID << " ������" << nC->name << " " << nC->sex << " �ֻ��ţ�" << nC->mobileNumber << endl << "\t";

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
					cout << "\tδ֪����,Ĭ�Ϸ�" << endl;
					strcpy(id, "0");
					continue;
				}
			}
		}
		//��Ʊ����
		Booking(airport, &airline, id, al);
	}
	else
	{
		cout << "\t�����ڸú���\n\t�Ƿ�Ҫ������Y/N" << endl;
		cout << "\t";
		c = Confirm();
		if (c == 1)
		{
			goto start;
		}
		else
		{
			cout << "\t�˳���Ʊ" << endl;
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
			cout << "\t���ڿͻ���" << (*c)->name << " " << (*c)->sex << " �ֻ��ţ�" << (*c)->mobileNumber << endl;
			cout << "\tǰ����Ʊ" << endl;

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
		cout << "\t�Բ��� , Ʊ������!! " << endl;
		cout << "\tԤ����Ʊ������(Y):";
		int cc = Confirm();
		char airplane[20];
		if (cc) {
			char jicang;
			cout << "\t��������ҪԤ���Ļ���:";
			cin >> jicang;
			while (jicang != 'F' && jicang != 'E' && jicang != 'B') {
				cout << "�����������������" << endl;
				cin >> jicang;
			}
			ofstream ofresult("data\\nexttickets.txt", ios::app);
			ofresult << ti->customerNumber << " " << ti->customerName << " " << ti->ticketNumber << " " << aa->airport << " " << aa->terminus << " " << aa->takeOffTime << " " << aa->arrivalTime << " " << jicang << aa->FPrice << endl;
			cout << "\t�򲹶�Ʊ�ɹ�" << endl;
		}
		return;
	}
	cout << "\t��ѡ������Ҫ���Ļ��գ�F,B,E)" << endl << "\t";
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
		cout << "\t������󣬲����ڻ���\n";
		goto intype;
	}
	strcpy(ti->airport, aa->airport);
	strcpy(ti->terminus, aa->terminus);
	strcpy(ti->takeOffTime, aa->takeOffTime);
	strcpy(ti->arrivalTime, aa->arrivalTime);

	(*tis).push_back(ti);
	aa->surplusTickets--;
	aa->customersInfo->push_back(ti);

	cout << "\tԤ���ɹ���" << endl;
	cout << "\t��Ʊ��Ϣ��" << endl << "\t����ţ�" << ti->ticketNumber << " �ͻ�֤���ţ� " << ti->customerNumber << " �ͻ������� " << ti->customerName
		<< " �۸� " << ti->price << " ��λ�� " << ti->cabinType << endl << "\t��㣺 " << ti->airport << " �յ㣺 " << ti->terminus
		<< " ���ʱ�䣺 " << ti->takeOffTime << " ����ʱ�䣺 " << ti->arrivalTime << endl;
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

	cout << "\t����������֤����: ";
	cin >> id;

	c = getCusInfoById(airport->customers, id);
	if (c == NULL)
	{
		cout << "\t������֤������Ϣ" << endl;
		cout << "\t";
		system("pause");
		return 0;
	}

	cout << "\t����֤����Ϣ��" << c->ID << "  " << c->name << " " << c->sex << " �ֻ��ţ� " << c->mobileNumber << endl;

	t = getTicketInfoById(airport->tickets, c->ID);
	if (t->size() == 0)
	{
		cout << "\t�����ڻ�Ʊ��Ϣ" << endl;
		cout << "\t";
		system("pause");
		return 0;
	}

	cout << endl << "\t��Ʊ��Ϣ��" << endl << endl;
	listTicketInfo::iterator ti;
	for (ti = t->begin(); ti != t->end(); ti++)
	{
		cout << "\t����ţ�" << (*ti)->ticketNumber << endl << "\t�ͻ�֤���ţ� " << (*ti)->customerNumber << " �ͻ������� " << (*ti)->customerName
			<< " �۸� " << (*ti)->price << " ��λ�� " << (*ti)->cabinType << endl << "\t��㣺 " << (*ti)->airport << " �յ㣺 " << (*ti)->terminus
			<< " ���ʱ�䣺 " << (*ti)->takeOffTime << " ����ʱ�䣺 " << (*ti)->arrivalTime << endl << endl;
	}

	cout << "\t��������Ҫ��Ʊ�ĺ����: ";
	cin >> num;

	tt = getTicketInfoByNum(t, num);
	if (tt == NULL)
	{
		cout << "\t�������ڸú����Ʊ" << endl;
		cout << "\t";
		system("pause");
		return 0;
	}

	cout << "\t��Ʊ��Ϣ��" << endl;
	cout << "\t����ţ�" << tt->ticketNumber << endl << "\t�ͻ�֤���ţ� " << tt->customerNumber << " �ͻ������� " << tt->customerName
		<< " �۸� " << tt->price << " ��λ�� " << tt->cabinType << endl << "\t��㣺 " << tt->airport << " �յ㣺 " << tt->terminus
		<< " ���ʱ�䣺 " << tt->takeOffTime << " ����ʱ�䣺 " << tt->arrivalTime << endl << endl;

	cout << "\tȷ��Ҫ�˵���Ʊ��Y/N" << endl << "\t";
	int cc = Confirm();
	if (cc == 1)
	{
		//��Ʊ
		Refund(airport, airline, tt);
		cout << "\t�Ƿ����򲹶�Ʊ��飨Y/N���� ";
		int flag= Confirm();
		if (flag == 1) {
			cout << "\n\n\t\t�򲹶�Ʊ���:" << endl;
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
		//����Ʊ
		cout << "\t�õģ�����Ʊ" << endl;
		cout << "\t";
		system("pause");
	}
	else
	{
		cout << "\tδ֪���룬����Ʊ" << endl;
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
	cout << "\tȷ�������Ϣ������(Y): ";
	int cc = Confirm();  //�����ж��Ƿ���Ӻ�����Ϣ
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
		cout << "\t�����뺽��ţ�";
		cin >> al->flightNumber;

		cout << "\t������ɻ��ͺţ�";
		cin >> al->planeNumber;
		cout << "\t���:";
		cin >> al->airport;
		cout << "\t�յ�:";
		cin >> al->terminus;
		cout << "\t���ʱ��:" << "(�����ʽ��:2019-07-08+22:00��";
		cin >> al->takeOffTime;
		cout << "\t����ʱ��:" << "(�����ʽ��:2019-07-08+22:00) ";
		cin >> al->arrivalTime;
		cout << "\t��ֱ���������Ʊ��:  (����λ���)" << endl;;
		cout << "\tͷ�Ȳ�(F):";
		cin >> al->FPrice;   cout << "\t��λ���:";   cin >> Fseat;
		cout << "\t�����(B):";
		cin >> al->BPrice;   cout << "\t��λ���:";   cin >> Bseat;
		cout << "\t���ò�(E):";
		cin >> al->EPrice;   cout << "\t��λ���:";   cin >> Eseat;
		ofresult1 << al->flightNumber << " " << al->planeNumber << " " << al->airport << " " << al->terminus<<" ";
		ofresult1 << al->takeOffTime << " " << al->arrivalTime << " " << al->FPrice << " " << al->BPrice << " " << al->EPrice <<" "<< endl;
		ofstream ofresult2("data\\planes.txt", ios::app);
		ofresult2 << al->planeNumber << " " << Fseat << " " << Bseat << " " << Eseat;
		cout << "\t����������(Y): ";
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
	char item[10][32] = { "���� airline","֤���� ID","���� Name","Ʊ�� Price","��λ Level",
		"��λ�� Seat","���� Form","�յ� To","���ʱ�� Departure","����ʱ�� Arrival" };
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
	cout << "\n\t  (ÿ���������ǰ���ٸ�)" << endl;

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
		cout << "\t��˳����ҵ��ֿɶ�Ʊ�ĺ򲹶�Ʊ��Ա:" << endl;
		cout << "\t+-------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
		char item[8][20] = { "֤���� ID","���� Name","����� Airplanenum","���� From","�յ� To","���ʱ�� Departure","����ʱ�� Arrival","��λ Level" };
		printf("\t|%14s|%14s|%18s|%18s|%18s|%18s|%18s|%14s|\n",item[0], item[1], item[2], item[3], item[4], item[5], item[6], item[7]);
		cout << "\t+-------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
		printf("\t|%14s|%14s|%18s|%18s|%18s|%18s|%18s|%14c|\n",id1[mubiao], name1[mubiao], airplanenum1[mubiao] , from1[mubiao], to1[mubiao], departure1[mubiao], arrival1[mubiao] ,seat1[mubiao]);
		cout << "\t+-------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
		cout << "\t�Ƿ�Ϊ��λ�ͻ���Ʊ��Y/N:";
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
		cout << "\tδ���ҵ��ֿɶ�Ʊ�ĺ򲹶�Ʊ��Ա" << endl;
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