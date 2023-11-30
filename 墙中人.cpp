// 墙中人.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "ColorfulConsoleIO.hpp"
#include "Interception/InterceptionHelper.h"//键鼠
#include "Op/libop.h" //libop接口的头文件
#include "ImGui/Draw.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>
#include "Windows.h"
#pragma comment(lib,"Interception/interception.lib")
#ifdef _M_X64
#pragma comment(lib,"Op/bin/x64/op_x64.lib") //64位lib
#else
#pragma comment(lib,"Op/bin/x86/op_x86.lib") //32位lib
#endif
using namespace std;

//坐标
typedef struct _Pos
{
	int X;
	int Y;
}Pos;

//获取系统时间，返回流字符串
static stringstream get_time()
{
	SYSTEMTIME m_time = { };
	GetLocalTime(&m_time);
	stringstream ss;
	ss << "[" << m_time.wYear << "年" << m_time.wMonth << "月" << m_time.wDay << "日" << right << setw(2) << setfill('0') << m_time.wHour << "时" << right << setw(2) << setfill('0') << m_time.wMinute << "分" << right << setw(2) << setfill('0') << m_time.wSecond << "秒]> ";
	return ss;
}

//获取屏幕尺寸
static Pos get_scren_size() 
{
	Pos ret = { GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN) };
	return ret;
}

//计算鼠标绝对坐标，传入鼠标屏幕坐标和屏幕分辨率返回绝对坐标
static Pos abs_pos(int X,int Y,int W,int H) 
{
	int AbsoluteX = static_cast<unsigned short>(X * 65535 / W);
	int AbsoluteY = static_cast<unsigned short>(Y * 65535 / H);
	Pos ret = { AbsoluteX,AbsoluteY };
	return ret;
}




int main()
{	
	libop						op;
	InterceptionHelper			ihelper;
	wstring						vers = op.Ver();
	HWND						hwnd_game = NULL, //游戏窗口句柄
								hwnd_console = NULL; //控制台窗口句柄
	wchar_t						title[1024]; //控制台标题					
	long						tmp = NULL; //临时返回变量
	long						retcmp = NULL; //返回比较结果
	long						state[2] = { NULL }; //窗口状态
	int							i = 0; //暂停次数
	int							ePos = 0;//位置状态
	bool						flag = NULL; //主动退出标志
	Pos	static					氧气 = { 0,0 },
								核桃 = abs_pos(390,280, get_scren_size().X, get_scren_size().Y);
	InterceptionKeyStroke		ESCpush = { 0x01 ,INTERCEPTION_KEY_DOWN ,NULL }, ESCpop = { 0x01,INTERCEPTION_KEY_UP,NULL };
	InterceptionMouseStroke		M1push = { INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN, INTERCEPTION_MOUSE_MOVE_RELATIVE ,NULL,0,0 ,NULL },
								M1pop = { INTERCEPTION_MOUSE_LEFT_BUTTON_UP, INTERCEPTION_MOUSE_MOVE_RELATIVE ,NULL,0,0 ,NULL },
								MPos = { INTERCEPTION_FILTER_MOUSE_MOVE, INTERCEPTION_MOUSE_MOVE_ABSOLUTE ,NULL,核桃.X,核桃.Y ,NULL};

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);//提升进程优先级
	cout << get_time().str(); wcout << "当前OP版本：" << vers << "\n";
	cout << get_time().str() << "你的屏幕尺寸为：" << get_scren_size().X << "X" << get_scren_size().Y << "\n";

	//获取键盘
	ihelper.GetNextKeyStroke();
	cout << get_time().str() << "请按下键盘任意键以获取设备ID\n";
	cout << get_time().str() << "你的键盘ID为：" << ihelper.GetKeyboradDevice() << "\n";
	cout << get_time().str() << "你按下的按键扫描码为：" << showbase << hex << ihelper.GetLastKeyStroke().code << dec << "\n";

	//获取鼠标
	ihelper.GetNextMouseStroke();
	cout << get_time().str() << "请按下鼠标任意键以获取设备ID\n";
	cout << get_time().str() << "你的鼠标ID为：" << ihelper.GetMouseDevice() << "\n";
	cout << get_time().str() << "你按下的按键扫描码为：" << showbase << hex << ihelper.GetLastMouseStroke().state << dec << "\n";

	//选择情况
	if (hwnd_game = FindWindow(L"WarframePublicEvolutionGfxD3D12", L"Warframe"))
	{
		GetConsoleTitle(title, 1024);
		hwnd_console = FindWindow(NULL, title);
		cout << get_time().str() << "游戏窗口句柄：" << hwnd_game << "\n"; 
		cout << get_time().str() << "控制台窗口句柄：" << hwnd_console << "\n";
	}else 
	{ 
		cout << get_time().str() << "获取游戏窗口句柄失败，游戏未打开？"; return -111;
	}
	cout << get_time().str() 
		<< "请按下键盘上1~4来选择你的目前情况\n"
		<< ">1:左上角小地图，普通生存\n"
		<< ">2:左上角小地图，虚空裂隙生存\n"
		<< ">3:叠层大地图，普通生存\n"
		<< ">4:叠层大地图，虚空裂隙生存\n"
		<< "\n请做选择...\n";
	while (ePos == 0 || (ePos <0x2 || ePos >0x5))
	{

		ePos = ihelper.GetNextKeyStroke().code;

		if (ePos != 0 )
		{
			cout << get_time().str(); 红字 cout << "选择错误\r"; 还原
		}	
	}
	cout << get_time().str() << "你选择了 "; 青字 cout << "[ ";
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

	Sleep(1000);
	cout << get_time().str() << "若要锁定鼠标到第一个核桃的位置，请按 "; 青底 黑字 cout << "[ O ]"; 还原 cout << " 键\n";
	cout << get_time().str() << "程序运行时会保持游戏窗口处于前台激活，若要手动退出请按 "; 青底 黑字 cout << "[ P ]"; 还原 cout << " 键\n";
	Sleep(1500);

	//配置快捷键
	map<string, void*> Map = { 
		{"flag",&flag} ,
		{"posx",&核桃.X},
		{"posy",&核桃.Y}
	};
	ihelper.HotKey(Map);

	//开始GUI线程
	thread thrgui(maingui);
	thrgui.detach();


	for (size_t i = 0; i < 10; i++)
	{
		while (retcmp == 0)
		{
			//主动退出
			if (flag == 1)
			{
				op.SetWindowState(reinterpret_cast<long&>(hwnd_console), 7, &tmp);
				cout << "\n" << get_time().str() << "已主动退出\n";
				return -1;
			}

			op.GetWindowState(reinterpret_cast<long&>(hwnd_game), 1, &state[1]); //检测是否激活
			op.GetWindowState(reinterpret_cast<long&>(hwnd_game), 2, &state[2]); //检测是否可见
			if (!(state[1] && state[0]))
			{
				//op.SetWindowState(reinterpret_cast<long&>(hwnd_game), 1, &tmp); //激活指定窗口
				//op.SetWindowState(reinterpret_cast<long&>(hwnd_game), 7, &tmp); //显示指定窗口
			}

			bool cmp[2] = { 0,0 };
			//多次循环防止因特效误触发
			for (size_t i = 0; i < 2; i++)
			{
				//比较颜色
				static long t_cmp = 0;
				op.CmpColor(氧气.X, 氧气.Y, L"C60608-0F0608|a82020-282020", 0.9, &t_cmp);
				cmp[i] = (static_cast<bool>(t_cmp));
				if (cmp[i] == 0) { cout << get_time().str(); 白底 黑字 cout << "氧气正常"; 还原 cout << "\r"; }
				else { cout << get_time().str(); 红底 cout << "氧气过低"; 还原 cout << "\r"; }
				Sleep(1000);
			}
			retcmp = cmp[0] && cmp[1];
		}
	

		if (retcmp) {
			cout << get_time().str(); 红底 cout << "氧气过低"; 还原 cout << "\n";
			ihelper.SendMouse(reinterpret_cast<InterceptionStroke*>(&M1push), 1);
			//interception_send(Mcontext, Mdevice, (InterceptionStroke*)&M1push, 1);
			Sleep(50);
			ihelper.SendMouse(reinterpret_cast<InterceptionStroke*>(&M1pop), 1);
			//interception_send(Mcontext, Mdevice, (InterceptionStroke*)&M1pop, 1);
			Sleep(150);
			ihelper.SendMouse(reinterpret_cast<InterceptionStroke*>(&ESCpush), 1);
			//interception_send(Kcontext, Kdevice, (InterceptionStroke*)&ESCpush, 1);//发送按键
			Sleep(50);
			ihelper.SendMouse(reinterpret_cast<InterceptionStroke*>(&ESCpop), 1);
			//interception_send(Kcontext, Kdevice, (InterceptionStroke*)&ESCpop, 1);
			cout << get_time().str() << "尝试暂停第 "; 绿字 cout << i + 1; 还原 cout << " 次\n";
			Sleep(1000);
			op.CmpColor(氧气.X, 氧气.Y, L"C60608-0F0608|a82020-282020", 0.9, &retcmp);//再次确认
		}

		if (!retcmp)
		{
			cout << get_time().str(); 绿底 黑字 cout << "暂停成功"; 还原 cout << "\n";
			return 666;
		}
		else
		{
			cout << get_time().str(); 黄底 黑字 cout << "暂停失败"; 还原 cout << "\n";
		}
	}
	op.SetWindowState(reinterpret_cast<long&>(hwnd_console), 7, &tmp);
	cout << get_time().str() << "暂停失败次数过多，已自动退出\n";
	return 0; 

}