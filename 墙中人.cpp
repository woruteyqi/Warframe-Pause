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
typedef struct {
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

//计算鼠标绝对坐标，传入鼠标屏幕坐标和屏幕分辨率返回绝对坐标
Pos AbsPos(int X,int Y,int W,int H) {
	int AbsoluteX = static_cast<unsigned short>(X * 65535 / W);
	int AbsoluteY = static_cast<unsigned short>(Y * 65535 / H);
	Pos ret = { AbsoluteX,AbsoluteY };
	return ret;
}



int main()
{	
	libop				op;
	wstring				vers = op.Ver();
	HWND				hwnd = NULL; //窗口句柄
	long				tmp = NULL; //临时返回变量
	long				retcmp = NULL; //返回比较结果
	long				state[2] = { NULL }; //窗口状态
	int					i = 0; //暂停次数
	int					ePos = 0;//位置状态
	int					flag = NULL; //主动退出标志
	Pos	static			氧气 = { 0,0 },
						核桃 = AbsPos(390,280,1920,1080);
	

	InterceptionStroke		Kstroke,Mstroke;
	InterceptionKeyStroke&	Kkstroke = *(InterceptionKeyStroke*)&Kstroke;
	InterceptionMouseStroke&Mkstroke = *(InterceptionMouseStroke*)&Mstroke;
	InterceptionContext		Kcontext,Mcontext;
	InterceptionDevice		Kdevice,Mdevice;
	InterceptionKeyStroke	ESCpush = { 0x01 ,INTERCEPTION_KEY_DOWN ,NULL }, ESCpop = { 0x01,INTERCEPTION_KEY_UP,NULL };
	InterceptionMouseStroke M1push = { INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN, INTERCEPTION_MOUSE_MOVE_RELATIVE ,NULL,0,0 ,NULL },
							M1pop= { INTERCEPTION_MOUSE_LEFT_BUTTON_UP, INTERCEPTION_MOUSE_MOVE_RELATIVE ,NULL,0,0 ,NULL },
							MPos = { INTERCEPTION_FILTER_MOUSE_MOVE, INTERCEPTION_MOUSE_MOVE_ABSOLUTE ,NULL,核桃.X,核桃.Y ,NULL};

	cout << GetTime().str(); wcout << "当前OP版本：" << vers << "\n";



	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);//提升进程优先级

	Kcontext = interception_create_context();
	interception_set_filter(Kcontext, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
	cout << GetTime().str() << "请按下键盘任意键以获取设备ID\n";
	Kdevice = interception_wait(Kcontext);
	interception_set_filter(Kcontext, interception_is_keyboard, NULL);
	interception_receive(Kcontext, Kdevice, &Kstroke, 1);
	cout << GetTime().str() << "你的键盘ID为：" << Kdevice << "\n";
	cout << GetTime().str() << "你按下按键的键盘扫描码为：" << showbase << hex << Kkstroke.code << dec << "\n";

	Mcontext = interception_create_context();
	interception_set_filter(Mcontext, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_ALL);
	cout << GetTime().str() << "请按下鼠标任意键以获取设备ID\n";
	Mdevice = interception_wait(Mcontext);
	interception_set_filter(Mcontext, interception_is_mouse, NULL);
	interception_receive(Mcontext, Mdevice, &Mstroke, 1);
	cout << GetTime().str() << "你的鼠标ID为：" << Mdevice << "\n";
	cout << GetTime().str() << "你按下按键的鼠标扫描码为：" << showbase << hex << Mkstroke.state << dec << "\n";

	


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
		interception_set_filter(Kcontext, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN);
		interception_wait(Kcontext);
		interception_receive(Kcontext, Kdevice, &Kstroke, 1);
		ePos = Kkstroke.code;
		interception_set_filter(Kcontext, interception_is_keyboard, NULL);

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
			氧气 = { 53, 237 };
			break;
		case 3:
			cout << "2:左上角小地图，虚空裂隙生存 ]\n"; 还原
			氧气 = { 53, 293 };
			break;
		case 4:
			cout << "3:叠层大地图，普通生存 ]\n"; 还原
			氧气 = { 53, 112 };
			break;
		case 5:
			cout << "4:叠层大地图，虚空裂隙生存 ]\n"; 还原
			氧气 = { 53, 167 };
			break;
	}

	op.Sleep(1000, &tmp);
	cout << GetTime().str() << "若要快速移动鼠标到第一个核桃位置，请按 "; 青底 黑字 cout << "[ O ]"; 还原 cout << " 键\n";
	cout << GetTime().str() << "程序运行时会保持游戏窗口处于前台激活，若要中断请按 "; 青底 黑字 cout << "[ P ]"; 还原 cout << " 键\n";
	op.Sleep(1500, &tmp);



	typedef struct test{
		int* flag;
		InterceptionContext* Mcontext;
		InterceptionDevice* Mdevice;
		InterceptionMouseStroke* MPos;
	};

	test ARG = {
		& flag,
		& Mcontext,
		& Mdevice,
		& MPos
	};
	
	//检测P键是否按下
	auto Fbreak = [](test *ARG)
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
					*(int*)ARG->flag = 1;
					break;
				}
				if (kstroke.code == 0x18)
				{
					interception_send(*(InterceptionContext*)ARG->Mcontext, *(InterceptionDevice*)ARG->Mdevice, (InterceptionStroke*)ARG->MPos, 1);
				}
			}
		};


	//开始线程
	thread thr(Fbreak,&ARG);

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
		op.CmpColor(氧气.X, 氧气.Y, L"C60608-0F0608|a82020-282020", 0.9, &retcmp);
		if (!retcmp) { cout << GetTime().str(); 白底 黑字 cout << "氧气正常"; 还原 cout << "\r"; }
		op.Sleep(1000, &tmp);
		//再加判断防止碰到红色特效
		op.CmpColor(氧气.X, 氧气.Y, L"C60608-0F0608|a82020-282020", 0.9, &retcmp);
		if (!retcmp) { cout << GetTime().str(); 白底 黑字 cout << "氧气正常"; 还原 cout << "\r"; }
	}


	if (retcmp) {
		cout << GetTime().str(); 红底 cout << "氧气过低"; 还原 cout << "\n";
		interception_send(Kcontext, Kdevice, (InterceptionStroke*)&ESCpush, 1);//发送按键
		interception_send(Kcontext, Kdevice, (InterceptionStroke*)&ESCpop, 1);
		cout << GetTime().str() << "尝试暂停第 "; 绿字 cout << ++i; 还原 cout << " 次\n";
		op.Sleep(1000, &tmp);
		op.CmpColor(氧气.X, 氧气.Y, L"C60608-0F0608|a82020-282020", 0.9, &retcmp);//再次确认
	}

	if (!retcmp) 
	{
		thr.detach();
		cout << GetTime().str(); 绿底 黑字 cout << "暂停成功"; 还原 cout << "\n";
		interception_destroy_context(Kcontext);
		return 666;
	}else
	{
		cout << GetTime().str(); 黄底 黑字 cout << "暂停失败"; 还原 cout << "\n";
	}
	

	if (i > 10)
	{
		thr.detach();
		cout << GetTime().str() << "暂停失败次数过多，已自动退出\n";
		interception_destroy_context(Kcontext);
		return 0; 
	}

	goto repuse;
}