#pragma once
#pragma warning(disable:4996) 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <list>
#include <fstream> 
#include <Windows.h>
#include<malloc.h>
#include<stdlib.h>
#include<string>
//#include <conio.h>
//#include <graphics.h>
#include <iostream>
using namespace std;

/**************************************************
					���ݽṹ
 **************************************************/

/*
 * Time
 */
typedef struct tm timeFormat;

/*
 * �ͻ���Ϣʵ��
 */
struct CusInfoStruct
{
	char* customerNumber;//�ͻ����
	char* name;//����
	char* sex;//�Ա�
	char* mobileNumber;//�ֻ���
	char* ID;//֤����
	int ticketVol;//��Ʊ�����ں򲹿ͻ���Ϊ����Ʊ����
};
typedef struct CusInfoStruct CusInfo;

/*
 * �û���Ϣ���Ա�
 */
typedef list<CusInfo*> listCusInfo;

/*
 * ��Ʊ��Ϣʵ��
 */
struct TicketInfoStruct
{
	char* ticketNumber;//��Ʊ��ţ�����ţ�
	char* customerNumber;//�ͻ����
	char* customerName;//�ͻ�����
	double price;//��Ʊ�۸�
	char cabinType;//��λ�ȼ���F:First Class,B:Bussiness Class,E:Economy Class or Coach)
	int seat;
	char* airport;//���վ
	char* terminus;//�յ�վ
	char* takeOffTime;//���ʱ��"date + time"
	char* arrivalTime;//����ʱ��

};
typedef struct TicketInfoStruct TicketInfo;

/*
 * ��Ʊ��Ϣ���Ա�
 */
typedef list<TicketInfo*> listTicketInfo;

/*
 * ������Ϣ
 */
struct AirlineStruct
{
	char* flightNumber;//������
	char* planeNumber;//�ͻ����
	char* airport;//���վ
	char* terminus;//�յ�վ
	char* takeOffTime;//���ʱ��
	char* arrivalTime;//����ʱ��
	int customersMaxinum;//�˿Ͷ�������ȡ��
	int surplusTickets;//��Ʊ���������ȡ��
	listTicketInfo* customersInfo;//��Ʊ��Ϣ�������ȡ��
	listTicketInfo* customersOfWaiting;//����Ϣ�������ȡ��
	double FPrice;//ͷ�Ȳռ۸�
	double BPrice;//����ռ۸�
	double EPrice;//���òռ۸�

};
typedef struct AirlineStruct Airline;

/*
 * �������Ա�
 */
typedef list<Airline*> listAirline;

/*
 * ����ʵ����Ϣ
 */
struct PlaneCabinStruct
{
	char cabinType;//��λ�ȼ���F:First Class,B:Bussiness Class,E:Economy Class or Coach)
	int cusMax;//�˿Ͷ���
   // int ** cabinMatrix;//��λ����1��ʾ��ѡ��0��ʾ�ѱ�ѡ
	int* seatNum;
};
typedef struct PlaneCabinStruct PlaneCabin;

/*
 * �ͻ���Ϣʵ��
 */

struct PlaneInfoStruct
{
	int customersMax;//�ܶ�����������
	char* planeNumber;//�ͻ����
	PlaneCabin* FSeat;//ͷ�Ȳ���λ 20
	PlaneCabin* BSeat;//�������λ 80
	PlaneCabin* ESeat;//���ò���λ 300

};
typedef struct PlaneInfoStruct PlaneInfo;

/*
 * �ͻ����Ա�
 */
typedef list<PlaneInfo*> listPlaneInfo;

/*
 * ��������
 */
struct localInfoStruct
{
	char* airplane;//���ڻ���
	//listAirline *airlines;//��ͨ����
	listCusInfo* customers;//�Ѵ��ڿͻ��б�
	listTicketInfo* tickets;//�Ѵ��ڻ�Ʊ
	listPlaneInfo* planes;//�ɻ���Ϣ
};
typedef struct localInfoStruct localInfo;


/*
 * ��ʼ��
 */

 //��ʼ��������Ϣ/��ȡ����
localInfo* InitAirport(listPlaneInfo* planes,
	listCusInfo* customers, listTicketInfo* tickets);

//������Ϣ��ʼ��
void InitAirline(listAirline& airline, localInfo* airport);

//�ͻ���Ϣ��ʼ��
void InitPlaneInfo(listPlaneInfo& planes);

//�ͻ���Ϣ��ʼ��
void InitCusInfo(listCusInfo& customers);

//��Ʊ��Ϣ��ʼ��
void InitTicketInfo(listTicketInfo& tickets);

//ͨ����Ʊ��ɻ���Ϣƥ���ȡ��Ʊ��
int SupTiOfPlane(listTicketInfo ticketList, int max, char* alname);

//ͨ����Ʊ��ɻ���Ϣƥ���ȡ�����
int CusMaxOfPlane(listPlaneInfo planes, char* planeName);

//��ȡ���κ���ͻ��б�
void GetCusList(listTicketInfo* cusList, listTicketInfo* tichets, char* fn);

//ѡ���������
int Service(listAirline airline, localInfo* airport);

//�˳�ϵͳ
int OutService();

/*
 * ҵ��
 */

 //�������
void PrintAirline(listAirline airline);

//��ѯ
int SeachService(listAirline airline, listAirline* a);

//��Ʊҵ��
int BookingService(listAirline airline, localInfo* airport);

//ȷ�ϲ���
int Confirm();

//վ����֤0/1
int IsAirport(listAirline airlines, char* airport);

//������֤0/1
int IsArilineNum(listAirline airlines, char* num);

//ͨ��վ���ѯ����
void SeachByFT(listAirline airlines, char* from, char* to, listAirline* als);

//��֤�û�
int CheckCustomer(localInfo* airport, char* ID);

//��Ʊ
void Booking(localInfo* airport, listAirline* airline, char* id, char* num);

//��ȡ�û�
CusInfo* getCusInfoById(listCusInfo* cus, char* id);

//��ȡ����
Airline* getAirlineByNum(listAirline* als, char* num);

//��ȡ�ɻ�Ʊ
listTicketInfo* getTicketInfoById(listTicketInfo* tkl, char* id);

TicketInfo* getTicketInfoByNum(listTicketInfo* tkl, char* num);

//��Ʊҵ��
int RefundService(localInfo* airport, listAirline* airline);

//��Ʊ
void Refund(localInfo* airport, listAirline* airline, TicketInfo* ticket);

//�����û���Ϣ
void SaveCustomer(listCusInfo* customers);

//�����Ʊ��Ϣ
void SaveTicket(listTicketInfo* tichets);

//���溽����Ϣ
void SaveAirline(listAirline* airlines);

//��ӻ�Ʊ
void AddAirline(listAirline& airline, localInfo* airport);

//�����Ʊ����
void Outputticket();

//��ѯ�򲹶�Ʊ����Ʊ����
int Nexttickets(char ticketnumber[]);

//��ȡ��ǰʱ�䲢��ʽ�����
//Ϊ����ɸѡ������׼��
string GetTime();
