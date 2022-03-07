#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include <iosfwd>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <direct.h>
#include "../../../Public/Core/Helper/Log.h"
#include "../../../Public/Core/Vector/String.h"


Log* Log::LogInstance = nullptr;
Log::Deletor Log::deletor;
using namespace  std;

Log::Log()
{

}

Log::~Log()
{
	if (sOutFile.is_open())
	{
		sOutFile.close();
	}
	sOutFile.clear();
}

Log* Log::Get()
{
	if (Log::LogInstance)
	{
		return Log::LogInstance;
	}
	Log::LogInstance= new Log();
	return LogInstance;
}

std::string Log::GetCurrentTimeAsString()
{
	const std::time_t now = std::time(0);
	std::tm tmNow;	
	localtime_s(&tmNow, &now);

	
	std::stringstream ss;
	ss << (tmNow.tm_year + 1900) << "_"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_mon + 1 << "_"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_mday << "-"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_hour << "_"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_min << "_"
		<< std::setfill('0') << std::setw(2) << tmNow.tm_sec<<".txt";
	return ss.str();
}
std::string Log::GetCurrentTimeAsStringWithBrackets() { return "[" + GetCurrentTimeAsString() + "]"; }
void  Log::InitConsole()
{
	//控制台支持中文
	wcout.imbue(locale(""));
	//sOutFile支持中文
	sOutFile.imbue(locale("chs"));
	{
		
		AllocConsole();	
		HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
		std::FILE *COutputHandle = _fdopen(SystemOutput, "w");

		
		HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
		int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
		std::FILE *CErrorHandle = _fdopen(SystemError, "w");

	
		HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
		int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
		std::FILE *CInputHandle = _fdopen(SystemInput, "r");

		ios::sync_with_stdio(true);

		freopen_s(&CInputHandle, "CONIN$", "r", stdin);
		freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
		freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();
	}
}

void Log::InitLogFile()
{
	wchar_t buff[1000];
	_wgetcwd(buff, 1000);
	const std::wstring CurrentPath = buff;
	const std::wstring LogFileDir = CurrentPath + L"\\Logs";
	FString errMsg = "";
	if (CreateDirectory(CurrentPath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{	
		if (CreateDirectory(LogFileDir.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			FString fileName(GetCurrentTimeAsString().c_str());
			FString temp(std::string(LogFileDir.begin(), LogFileDir.end()).c_str());
			filePath = temp + "\\" + fileName;
			sOutFile.open(*filePath);
			if (sOutFile)
			{
				FString msg = FString(GetCurrentTimeAsStringWithBrackets().c_str()) + "[Log] " + " initialized: " + L"\n";
				sOutFile <<*msg;
				wcout << *msg ;
			}
			else
			{
				errMsg = fileName+ "  Cannot open log file ";
			}
		}
		else
		{
			errMsg =FString(std::string(LogFileDir.begin(), LogFileDir.end()).c_str()) +"   Failed to create directory " ;
		}
	}
}
void Log::Initialize()
{
//	InitConsole();
//	InitLogFile();
}

void Log::Exit()
{
	std::string msg = GetCurrentTimeAsStringWithBrackets() + "[Log] Exit()";
	if (sOutFile.is_open())
	{
		sOutFile << std::wstring(msg.begin(), msg.end()).c_str();
		sOutFile.close();
	}
	cout << msg;
	OutputDebugString(std::wstring(msg.begin(),msg.end()).c_str());
}

void Log::OpenFile()
{
	wchar_t buff[1000];
	_wgetcwd(buff, 1000);
	const std::wstring CurrentPath = buff;
	const std::wstring LogFileDir = CurrentPath + L"\\Logs";
	FString errMsg = "";
	if (CreateDirectory(CurrentPath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		if (CreateDirectory(LogFileDir.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			FString fileName(GetCurrentTimeAsString().c_str());
			FString temp(std::string(LogFileDir.begin(), LogFileDir.end()).c_str());
			filePath = temp + "\\" + fileName;
			sOutFile.open(*filePath);
			if (sOutFile)
			{
				Log_Info(FString(GetCurrentTimeAsStringWithBrackets().c_str()) + "[Log] " + " initialized: " + filePath + L"\n");
			}
			else
			{
				Log_Error(fileName + "  Cannot open log file ");
			}
		}
		else
		{
		        Log_Error(FString(std::string(LogFileDir.begin(), LogFileDir.end()).c_str()) + "   Failed to create directory ");
		}
	}
}

void Log::Info(FString s)
{
	logData.push_back(pair<int,FString>(0, FString(GetCurrentTimeAsStringWithBrackets().c_str()) + "[INFO]: " + s + L"\n"));
}

void Log::Info(const char* s)
{
	logData.push_back(pair<int, FString>(0,FString(GetCurrentTimeAsStringWithBrackets().c_str()) + "[INFO]: " + s + L"\n"));
}

void Log::Error(FString s)
{
	logData.push_back(pair<int, FString>(2,FString(GetCurrentTimeAsStringWithBrackets().c_str()) + "[ERROR]: "+ s + L"\n"));
}

void Log::Error(const char* s)
{
	logData.push_back(pair<int, FString>(2,FString(GetCurrentTimeAsStringWithBrackets().c_str()) + "[ERROR]: " +s + L"\n"));
}

void Log::Warning(FString s)
{
	logData.push_back(pair<int, FString>(1,FString(GetCurrentTimeAsStringWithBrackets().c_str()) + "[WARNING]: " + s + L"\n"));
}

void Log::Warning(const char* s)
{
	logData.push_back(pair<int, FString>(1,FString(GetCurrentTimeAsStringWithBrackets().c_str()) + "[WARNING]: " + s + L"\n"));
}

TArray <std::pair<int, FString>>& Log::GetData()
{
	return logData;
}

