#pragma once
#include "interception.h"
#include <map>
#include <string>
#include <thread>
class InterceptionHelper
{
public:
	InterceptionHelper();
	~InterceptionHelper();
	InterceptionDevice		GetKeyboradDevice();
	InterceptionDevice		GetMouseDevice();
	InterceptionKeyStroke	GetLastKeyStroke();
	InterceptionMouseStroke GetLastMouseStroke();
	InterceptionKeyStroke	GetNextKeyStroke();//兼初始化Keydevice
	InterceptionMouseStroke GetNextMouseStroke();//兼初始化Mousedevice
	int						SendKeyborad(InterceptionStroke *stroke,unsigned int nstroke);
	int						SendMouse(InterceptionStroke* stroke, unsigned int nstroke);
	//MAP容器存放要修改变量的名称与指针
	void					HotKey(std::map<std::string,void *> P);
private:

private:
	InterceptionStroke			Kstroke{}, Mstroke{};
	InterceptionContext			Kcontext, Mcontext;
	InterceptionDevice			Kdevice, Mdevice;
	InterceptionKeyStroke&		Kkstroke = *(InterceptionKeyStroke*)&Kstroke;
	InterceptionMouseStroke&	Mkstroke = *(InterceptionMouseStroke*)&Mstroke;
};

