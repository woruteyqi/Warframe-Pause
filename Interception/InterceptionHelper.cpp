#include "InterceptionHelper.h"
#include "KeyCodeEnum.h"
#include "Windows.h"
#include "Global.h"
#pragma comment(lib,"Interception/interception.lib")
InterceptionHelper::InterceptionHelper()
{
	Kcontext = interception_create_context();
	Mcontext = interception_create_context();
	Kdevice = -1;
	Mdevice = -1;
}

InterceptionHelper::~InterceptionHelper()
{
	interception_destroy_context(Kcontext);
	interception_destroy_context(Mcontext);
}

InterceptionKeyStroke InterceptionHelper::GetNextKeyStroke()
{
	interception_set_filter(Kcontext, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN);
	interception_receive(Kcontext, Kdevice = interception_wait(Kcontext), &Kstroke, 1);
	interception_set_filter(Kcontext, interception_is_keyboard, INTERCEPTION_FILTER_KEY_NONE);
	return Kkstroke;
}
InterceptionMouseStroke InterceptionHelper::GetNextMouseStroke()

{
	interception_set_filter(Mcontext, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_ALL);	
	interception_receive(Mcontext, Mdevice = interception_wait(Mcontext), &Mstroke, 1);
	interception_set_filter(Mcontext, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_NONE);
	return Mkstroke;
}

InterceptionDevice InterceptionHelper::GetKeyboradDevice()
{
	return Kdevice;
}

InterceptionDevice InterceptionHelper::GetMouseDevice()
{
	return Mdevice;
}

InterceptionKeyStroke InterceptionHelper::GetLastKeyStroke()
{
	return Kkstroke;
}
InterceptionMouseStroke InterceptionHelper::GetLastMouseStroke()
{
	return Mkstroke;
}

int	InterceptionHelper::SendKeyborad(InterceptionStroke* stroke, unsigned int nstroke = 1)
{
	return interception_send(Kcontext, Kdevice, stroke, nstroke);
}
int	InterceptionHelper::SendMouse(InterceptionStroke* stroke, unsigned int nstroke = 1 )
{
	return interception_send(Mcontext, Mdevice, stroke, nstroke);
}

void InterceptionHelper::HotKey(std::map<std::string, void*> P)
{
	
	auto hotkeythread = [this](std::map<std::string, void*> P)
	{	


		interception_set_filter(Kcontext, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN);
		while (interception_receive(Kcontext, Kdevice = interception_wait(Kcontext), &Kstroke, 1) > 0)
		{
			extern bool g_console;
			extern int	g_lock;
			switch (Kkstroke.code)
			{
				case KeyCode::KEY_GRAVE:
					if (g_console == 0)g_console = 1;
					else g_console = 0;
					break;
				case KeyCode::KEY_O:
					if (P.find("posx") != P.end() && P.find("posy") != P.end())
					{
						int x = *(int*)P["posx"],
							y = *(int*)P["posy"];

						if (g_lock == 0) 
						{
							g_lock = 1;
							InterceptionMouseStroke Pos = { INTERCEPTION_FILTER_MOUSE_MOVE, INTERCEPTION_MOUSE_MOVE_ABSOLUTE ,0, *(int*)P["posx"], *(int*)P["posy"] ,0 };
							interception_send(Mcontext, Mdevice, reinterpret_cast<InterceptionStroke*>(&Pos), 1);
							interception_set_filter(Mcontext, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_MOVE);
						}else
						{
							interception_set_filter(Mcontext, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_NONE);
							g_lock = 0;
						}
							
					}
					break;

				case KeyCode::KEY_P:
					if (P.find("flag") != P.end()) 
					{
						*(int*)P["flag"]= 1;
					}			
					break;

				default:
					interception_send(Kcontext, Kdevice, reinterpret_cast<InterceptionStroke*>(Kstroke), 1);
					break;
			}

		}
		interception_set_filter(Kcontext, interception_is_keyboard, INTERCEPTION_FILTER_KEY_NONE);
	};

	std::thread htk(hotkeythread, P);
	htk.detach();
}