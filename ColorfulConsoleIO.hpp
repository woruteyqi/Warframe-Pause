#include <iostream>
#include <string>
#define IS_WINDOWS defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) && !defined(__CYGWIN__)

#if IS_WINDOWS
	#include <Windows.h>
#endif // OS_TYPE_WINDOWS_CC

// Enum class for console text color
enum class ConsoleColor
{
	Green, Red, Blue, White, Black, Yellow, Purple, Gray, Cyan, None,
	GreenIntensity, RedIntensity, BlueIntensity, WhiteIntensity, BlackIntensity, YellowIntensity, PurpleIntensity, GrayIntensity, CyanIntensity
};

// Enum class for console background color
enum class ConsoleBackgroundColor
{
	Green, Red, Blue, White, Black, Yellow, Purple, Gray, Cyan, None
};

#if IS_WINDOWS
	// Get the Windows color code for a given ConsoleColor
	WORD GetColorCode(ConsoleColor color)
	{
		switch (color)
		{
			case ConsoleColor::Green: return FOREGROUND_GREEN;
			case ConsoleColor::Black: return 0;
			case ConsoleColor::Blue: return FOREGROUND_BLUE;
			case ConsoleColor::Gray: return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			case ConsoleColor::Purple: return FOREGROUND_BLUE | FOREGROUND_RED;
			case ConsoleColor::Red: return FOREGROUND_RED;
			case ConsoleColor::White: return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
			case ConsoleColor::Cyan: return FOREGROUND_BLUE | FOREGROUND_GREEN;
			case ConsoleColor::Yellow: return FOREGROUND_RED | FOREGROUND_GREEN;
			case ConsoleColor::None: return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
			case ConsoleColor::GreenIntensity: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			case ConsoleColor::BlackIntensity: return 0;
			case ConsoleColor::BlueIntensity: return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			case ConsoleColor::GrayIntensity: return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			case ConsoleColor::PurpleIntensity: return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
			case ConsoleColor::RedIntensity: return FOREGROUND_RED | FOREGROUND_INTENSITY;
			case ConsoleColor::WhiteIntensity: return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			case ConsoleColor::YellowIntensity: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			case ConsoleColor::CyanIntensity: return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			default: return 0;
		}
	}
#else
	// Get the ANSI escape code for a given ConsoleColor
	std::wstring GetColorCode(ConsoleColor color)
	{
		switch (color)
		{
			case ConsoleColor::Green: return L"\033[32m";
			case ConsoleColor::Black: return L"\033[30m";
			case ConsoleColor::Blue: return L"\033[34m";
			case ConsoleColor::Gray: return L"\033[37m";
			case ConsoleColor::Purple: return L"\033[35m";
			case ConsoleColor::Red: return L"\033[31m";
			case ConsoleColor::White: return L"\033[37m";
			case ConsoleColor::Cyan: return L"\033[36m";
			case ConsoleColor::Yellow: return L"\033[33m";
			case ConsoleColor::None: return L"\033[0m";
			case ConsoleColor::GreenIntensity: return L"\033[32m;1m";
			case ConsoleColor::BlackIntensity: return L"\033[30m;1m";
			case ConsoleColor::BlueIntensity: return L"\033[34m;1m";
			case ConsoleColor::GrayIntensity: return L"\033[37m;1m";
			case ConsoleColor::PurpleIntensity: return L"\033[35m;1m";
			case ConsoleColor::RedIntensity: return L"\033[31m;1m";
			case ConsoleColor::WhiteIntensity: return L"\033[37m;1m";
			case ConsoleColor::YellowIntensity: return L"\033[33m;1m";
			case ConsoleColor::CyanIntensity: return L"\033[36m;1m";
			default: return 0;
		}
	}
#endif

#if IS_WINDOWS
	// Get the Windows color code for a given ConsoleBackgroundColor
	WORD GetBackgroundColorCode(ConsoleBackgroundColor color)
	{
		switch (color)
		{
			case ConsoleBackgroundColor::Green: return BACKGROUND_GREEN;
			case ConsoleBackgroundColor::Black: return 0;
			case ConsoleBackgroundColor::Blue: return BACKGROUND_BLUE;
			case ConsoleBackgroundColor::Gray: return 0;
			case ConsoleBackgroundColor::Purple: return BACKGROUND_RED | BACKGROUND_BLUE;
			case ConsoleBackgroundColor::Red: return BACKGROUND_RED;
			case ConsoleBackgroundColor::White: return BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
			case ConsoleBackgroundColor::Cyan: return BACKGROUND_BLUE | BACKGROUND_GREEN;
			case ConsoleBackgroundColor::Yellow: return BACKGROUND_RED | BACKGROUND_GREEN;
			case ConsoleBackgroundColor::None: return 0;
			default: return 0;
		}
	}
#else
	// Get the ANSI escape code for a given ConsoleBackgroundColor
	std::wstring GetBackgroundColorCode(ConsoleBackgroundColor color)
	{
		switch (color)
		{
			case ConsoleBackgroundColor::Green: return L"\033[42m";
			case ConsoleBackgroundColor::Black: return L"\033[40m";
			case ConsoleBackgroundColor::Blue: return L"\033[44m";
			case ConsoleBackgroundColor::Gray: return L"\033[40m";
			case ConsoleBackgroundColor::Purple: return L"\033[45m";
			case ConsoleBackgroundColor::Red: return L"\033[41m";
			case ConsoleBackgroundColor::White: return L"\033[47m";
			case ConsoleBackgroundColor::Cyan: return L"\033[46m";
			case ConsoleBackgroundColor::Yellow: return L"\033[43m";
			case ConsoleBackgroundColor::None: return L"\033[40m";
			default: return 0;
		}
	}
#endif


// Operator overloading for console text color
std::wostream& operator<< (std::wostream& os, ConsoleColor data)
{
#if IS_WINDOWS
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, GetColorCode(data));
#else 
	std::wcout << GetColorCode(data);
#endif

	return os;
}


// Operator overloading for console background color
std::wostream& operator<< (std::wostream& os, ConsoleBackgroundColor data)
{
#if IS_WINDOWS
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, GetBackgroundColorCode(data));
#else 
	std::wcout << GetBackgroundColorCode(data);
#endif

	return os;
}

#define ºì×Ö std::wcout << ConsoleColor::Red;
#define ÂÌ×Ö std::wcout << ConsoleColor::Green;
#define À¶×Ö std::wcout << ConsoleColor::Blue;
#define ºÚ×Ö std::wcout << ConsoleColor::Black;
#define °××Ö std::wcout << ConsoleColor::White;
#define ×Ï×Ö std::wcout << ConsoleColor::Purple;
#define »Æ×Ö std::wcout << ConsoleColor::Yellow;
#define Çà×Ö std::wcout << ConsoleColor::Cyan;

#define ºìµ× std::wcout << ConsoleBackgroundColor::Red;
#define ÂÌµ× std::wcout << ConsoleBackgroundColor::Green;
#define À¶µ× std::wcout << ConsoleBackgroundColor::Blue;
#define ºÚµ× std::wcout << ConsoleBackgroundColor::Black;
#define °×µ× std::wcout << ConsoleBackgroundColor::White;
#define ×Ïµ× std::wcout << ConsoleBackgroundColor::Purple;
#define »Æµ× std::wcout << ConsoleBackgroundColor::Yellow;
#define Çàµ× std::wcout << ConsoleBackgroundColor::Cyan;

#define »¹Ô­ wcout << ConsoleBackgroundColor::None << ConsoleColor::None;