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
					数据结构
 **************************************************/

/*
 * Time
 */
typedef struct tm timeFormat;

/*
 * 客户信息实体
 */
struct CusInfoStruct
{
	char* customerNumber;//客户编号
	char* name;//姓名
	char* sex;//性别
	char* mobileNumber;//手机号
	char* ID;//证件号
	int ticketVol;//订票量（在候补客户中为所需票量）
};
typedef struct CusInfoStruct CusInfo;

/*
 * 用户信息线性表
 */
typedef list<CusInfo*> listCusInfo;

/*
 * 订票信息实体
 */
struct TicketInfoStruct
{
	char* ticketNumber;//订票编号（航班号）
	char* customerNumber;//客户编号
	char* customerName;//客户姓名
	double price;//机票价格
	char cabinType;//舱位等级（F:First Class,B:Bussiness Class,E:Economy Class or Coach)
	int seat;
	char* airport;//起点站
	char* terminus;//终点站
	char* takeOffTime;//起飞时间"date + time"
	char* arrivalTime;//降落时间

};
typedef struct TicketInfoStruct TicketInfo;

/*
 * 订票信息线性表
 */
typedef list<TicketInfo*> listTicketInfo;

/*
 * 航线信息
 */
struct AirlineStruct
{
	char* flightNumber;//航班编号
	char* planeNumber;//客机编号
	char* airport;//起点站
	char* terminus;//终点站
	char* takeOffTime;//起飞时间
	char* arrivalTime;//降落时间
	int customersMaxinum;//乘客定额（计算获取）
	int surplusTickets;//余票量（计算获取）
	listTicketInfo* customersInfo;//机票信息（计算获取）
	listTicketInfo* customersOfWaiting;//候补信息（计算获取）
	double FPrice;//头等舱价格
	double BPrice;//商务舱价格
	double EPrice;//经济舱价格

};
typedef struct AirlineStruct Airline;

/*
 * 航班线性表
 */
typedef list<Airline*> listAirline;

/*
 * 机舱实体信息
 */
struct PlaneCabinStruct
{
	char cabinType;//舱位等级（F:First Class,B:Bussiness Class,E:Economy Class or Coach)
	int cusMax;//乘客定额
   // int ** cabinMatrix;//座位矩阵1表示可选，0表示已被选
	int* seatNum;
};
typedef struct PlaneCabinStruct PlaneCabin;

/*
 * 客机信息实体
 */

struct PlaneInfoStruct
{
	int customersMax;//总定额（计算求出）
	char* planeNumber;//客机编号
	PlaneCabin* FSeat;//头等舱座位 20
	PlaneCabin* BSeat;//商务舱座位 80
	PlaneCabin* ESeat;//经济舱座位 300

};
typedef struct PlaneInfoStruct PlaneInfo;

/*
 * 客机线性表
 */
typedef list<PlaneInfo*> listPlaneInfo;

/*
 * 本地数据
 */
struct localInfoStruct
{
	char* airplane;//所在机场
	//listAirline *airlines;//开通航班
	listCusInfo* customers;//已存在客户列表
	listTicketInfo* tickets;//已存在机票
	listPlaneInfo* planes;//飞机信息
};
typedef struct localInfoStruct localInfo;


/*
 * 初始化
 */

 //初始化航班信息/读取数据
localInfo* InitAirport(listPlaneInfo* planes,
	listCusInfo* customers, listTicketInfo* tickets);

//航线信息初始化
void InitAirline(listAirline& airline, localInfo* airport);

//客机信息初始化
void InitPlaneInfo(listPlaneInfo& planes);

//客户信息初始化
void InitCusInfo(listCusInfo& customers);

//机票信息初始化
void InitTicketInfo(listTicketInfo& tickets);

//通过机票与飞机信息匹配获取余票量
int SupTiOfPlane(listTicketInfo ticketList, int max, char* alname);

//通过机票与飞机信息匹配获取额定人数
int CusMaxOfPlane(listPlaneInfo planes, char* planeName);

//获取本次航班客户列表
void GetCusList(listTicketInfo* cusList, listTicketInfo* tichets, char* fn);

//选择操作分析
int Service(listAirline airline, localInfo* airport);

//退出系统
int OutService();

/*
 * 业务
 */

 //输出航班
void PrintAirline(listAirline airline);

//查询
int SeachService(listAirline airline, listAirline* a);

//订票业务
int BookingService(listAirline airline, localInfo* airport);

//确认操作
int Confirm();

//站点验证0/1
int IsAirport(listAirline airlines, char* airport);

//航班验证0/1
int IsArilineNum(listAirline airlines, char* num);

//通过站点查询航线
void SeachByFT(listAirline airlines, char* from, char* to, listAirline* als);

//验证用户
int CheckCustomer(localInfo* airport, char* ID);

//订票
void Booking(localInfo* airport, listAirline* airline, char* id, char* num);

//获取用户
CusInfo* getCusInfoById(listCusInfo* cus, char* id);

//获取航班
Airline* getAirlineByNum(listAirline* als, char* num);

//获取飞机票
listTicketInfo* getTicketInfoById(listTicketInfo* tkl, char* id);

TicketInfo* getTicketInfoByNum(listTicketInfo* tkl, char* num);

//退票业务
int RefundService(localInfo* airport, listAirline* airline);

//退票
void Refund(localInfo* airport, listAirline* airline, TicketInfo* ticket);

//保存用户信息
void SaveCustomer(listCusInfo* customers);

//保存机票信息
void SaveTicket(listTicketInfo* tichets);

//保存航班信息
void SaveAirline(listAirline* airlines);

//添加机票
void AddAirline(listAirline& airline, localInfo* airport);

//输出订票名单
void Outputticket();

//查询候补订票并订票操作
int Nexttickets(char ticketnumber[]);

//获取当前时间并格式化输出
//为后面筛选航班做准备
string GetTime();
