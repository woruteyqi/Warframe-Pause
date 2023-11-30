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
	InterceptionKeyStroke	GetNextKeyStroke();//���ʼ��Keydevice
	InterceptionMouseStroke GetNextMouseStroke();//���ʼ��Mousedevice
	int						SendKeyborad(InterceptionStroke *stroke,unsigned int nstroke);
	int						SendMouse(InterceptionStroke* stroke, unsigned int nstroke);
	//MAP�������Ҫ�޸ı�����������ָ��
	void					HotKey(std::map<std::string,void *> P);
private:

private:
	InterceptionStroke			Kstroke{}, Mstroke{};
	InterceptionContext			Kcontext, Mcontext;
	InterceptionDevice			Kdevice, Mdevice;
	InterceptionKeyStroke&		Kkstroke = *(InterceptionKeyStroke*)&Kstroke;
	InterceptionMouseStroke&	Mkstroke = *(InterceptionMouseStroke*)&Mstroke;
};

