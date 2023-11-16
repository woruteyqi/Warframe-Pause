// 墙中人.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "ColorfulConsoleIO.hpp"
#include "Interception/interception.h"//键鼠
#include "Op/libop.h" //libop接口的头文件
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>
#include <Windows.h>
#pragma comment(lib,"Interception/interception.lib")
#ifdef _M_X64
#pragma comment(lib,"Op/bin/x64/op_x64.lib") //64位lib
#else
#pragma comment(lib,"Op/bin/x86/op_x86.lib") //32位lib
#endif



using namespace std;

//坐标
struct Pos {
	int X;
	int Y;
}Pos;


//获取系统时间，返回流字符串
static stringstream GetTime()
{
	SYSTEMTIME m_time = { };
	GetLocalTime(&m_time);
	stringstream ss;
	ss << "[" << m_time.wYear << "年" << m_time.wMonth << "月" << m_time.wDay << "日" << right << setw(2) << setfill('0') << m_time.wHour << "时" << right << setw(2) << setfill('0') << m_time.wMinute << "分" << right << setw(2) << setfill('0') << m_time.wSecond << "秒]> ";
	return ss;
}





int main()
{	
	libop op;
	wstring vers = op.Ver();
	HWND hwnd = NULL; //窗口句柄
	long tmp = NULL; //临时返回变量
	long retcmp = NULL; //返回比较结果
	long state[2] = { NULL }; //窗口状态
	int i = 0; //暂停次数
	int ePos = 0;//位置状态
	int flag = NULL; //主动退出标志

	InterceptionStroke Stroke;
	InterceptionKeyStroke& kstroke = *(InterceptionKeyStroke*)&Stroke;
	InterceptionContext context;
	InterceptionDevice device;
	InterceptionKeyStroke ESCpush = { 0x01 ,INTERCEPTION_KEY_DOWN ,0 }, ESCpop = { 0x01,INTERCEPTION_KEY_UP,0 };

	cout << GetTime().str();
	wcout << "当前OP版本：" << vers << "\n";

	//提升进程优先级
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	context = interception_create_context();
	interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
	cout << GetTime().str() << "请按下键盘任意键以获取设备ID\n";
	device = interception_wait(context);
	interception_set_filter(context, interception_is_keyboard, NULL);
	interception_receive(context, device, &Stroke, 1);
	cout << GetTime().str() << "你的键盘ID为：" << device << "\n";
	cout << GetTime().str() << "你按下按键的键盘扫描码为：" << showbase << hex << kstroke.code << dec << "\n";
	op.Sleep(1000, &tmp);
	if (hwnd = FindWindowA("WarframePublicEvolutionGfxD3D12", "Warframe")) { cout << GetTime().str()<< "窗口句柄：" << hwnd << "\n"; }
	else { cout << GetTime().str() << "获取窗口句柄失败，游戏未打开？"; return -111; }
	op.Sleep(1000, &tmp);
	cout << GetTime().str() 
		<< "请按下键盘上1~4来选择你的目前情况\n"
		<< ">1:左上角小地图，普通生存\n"
		<< ">2:左上角小地图，虚空裂隙生存\n"
		<< ">3:叠层大地图，普通生存\n"
		<< ">4:叠层大地图，虚空裂隙生存\n"
		<< "\n请做选择...\n";
	while (ePos == 0 || (ePos <2 || ePos >5))
	{
		interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN);
		interception_wait(context);
		interception_receive(context, device, &Stroke, 1);
		ePos = kstroke.code;
		interception_set_filter(context, interception_is_keyboard, NULL);

		if (ePos != 0 )
		{
			cout << GetTime().str(); 红字 cout << "选择错误\r"; 还原
		}	
	}

	cout << GetTime().str() << "你选择了 "; 青字 cout << "[ ";

	switch (ePos)
	{
		case 2:
			cout << "1:左上角小地图，普通生存 ]\n";还原
			Pos = { Pos.X = 53,Pos.Y = 237 };
			break;
		case 3:
			cout << "2:左上角小地图，虚空裂隙生存 ]\n"; 还原
			Pos = { Pos.X = 53,Pos.Y = 293 };
			break;
		case 4:
			cout << "3:叠层大地图，普通生存 ]\n"; 还原
			Pos = { Pos.X = 53,Pos.Y = 112 };
			break;
		case 5:
			cout << "4:叠层大地图，虚空裂隙生存 ]\n"; 还原
			Pos = { Pos.X = 53,Pos.Y = 168 };
			break;
	}

	op.Sleep(1000, &tmp);
	cout << GetTime().str() << "程序运行时会保持游戏窗口处于前台激活，若要中断请按 "; 青底 黑字 cout << "[ P ]"; 还原 cout << " 键\n";
	op.Sleep(1500, &tmp);

	//检测P键是否按下
	auto Fbreak = [](int *flag)
		{
			InterceptionContext mcontext = interception_create_context();
			InterceptionDevice mdevice;
			InterceptionStroke mstroke;
			interception_set_filter(mcontext, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);

			while (interception_receive(mcontext, mdevice = interception_wait(mcontext), &mstroke, 1) > 0)
			{
				InterceptionKeyStroke& kstroke = *(InterceptionKeyStroke*)&mstroke;
				interception_send(mcontext, mdevice, &mstroke, 1);
				if (kstroke.code == 0x19)
				{
					interception_destroy_context(mcontext);
					*flag = 1;
					break;
				}
			}
		};

	//开始线程
	thread thr(Fbreak,&flag);

repuse:
	
	while (retcmp == 0)
	{	
		//主动退出
		if (flag == 1)
		{
			thr.detach();//销毁线程
			cout << "\n" << GetTime().str() << "已主动退出\n";
			return -1;
		} 

		op.GetWindowState((long)&hwnd, 1, &state[1]); //检测是否激活
		op.GetWindowState((long)&hwnd, 2, &state[2]); //检测是否可见
		if (!(state[1] && state[0])) 
		{
			op.SetWindowState((long)hwnd, 1, &tmp); //激活指定窗口
			op.SetWindowState((long)hwnd, 7, &tmp); //显示指定窗口
		}
		//比较颜色
		op.CmpColor(Pos.X,Pos.Y, L"C60608-0F0608|B02828-102828", 0.9, &retcmp);
		if (!retcmp) { cout << GetTime().str(); 白底 黑字 cout << "氧气正常"; 还原 cout << "\r"; }
		op.Sleep(1000, &tmp);				
	}

	cout << GetTime().str(); 红底 cout << "氧气过低，尝试暂停"; 还原 cout << "\n";
	interception_send(context, device, (InterceptionStroke*)&ESCpush, 1);//发送按键
	interception_send(context, device, (InterceptionStroke*)&ESCpop, 1);
	
	cout << GetTime().str() << "尝试暂停第 "; 绿字 cout << ++i; 还原 cout << " 次\n";

	op.Sleep(1000, &tmp);

	op.CmpColor(Pos.X, Pos.Y, L"C60608-0F0608|B02828-102828", 0.9, &retcmp);//再次确认


	if (retcmp == 0) 
	{
		thr.detach();
		cout << GetTime().str(); 绿底 黑字 cout << "暂停成功"; 还原 cout << "\n";
		interception_destroy_context(context);
		return 666;
	}else
	{
		cout << GetTime().str(); 黄底 黑字 cout << "暂停失败"; 还原 cout << "\n";
	}
	
	

	if (i > 10)
	{
		thr.detach();
		cout << GetTime().str() << "暂停失败次数过多，已自动退出\n";
		interception_destroy_context(context);
		return 0; 
	}

	goto repuse;
}