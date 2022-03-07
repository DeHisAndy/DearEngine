#pragma once


#include <string>
#include<fstream>
#include "../Vector/String.h"
#include "../Object/Object/Object.h"
#include <utility>

#define VARIADIC_LOG_FN(FN_NAME)\
template<class... Args>\
void FN_NAME(const char* format, Args&&... args)\
{\
	char msg[LEN_MSG_BUFFER];\
	sprintf_s(msg, format, args...);\
	##FN_NAME(std::string(msg));\
}

class Log
{
private:
	Log();
	~Log();
	static Log* LogInstance;
private:
	class Deletor {
	public:
		~Deletor() {
			if (Log::LogInstance != nullptr)
				delete Log::LogInstance;
		}
	};
	FString filePath;
	static Deletor deletor;
public:

static	Log* Get();

	 std::wofstream sOutFile;
	enum Mode : unsigned	// unused.
	{
		NONE = 0,	// Visual Studio Output Window
		CONSOLE = 1,	// Separate Console Window
		FILE = 2,	// Log File in %APPDATA%\VQEngine\DATE-VQEngineLog.txt

		CONSOLE_AND_FILE = CONSOLE | FILE,	// Both Console Window & Log File
	};

	//---------------------------------------------------------------------------------------------

	size_t LEN_MSG_BUFFER = 2048;

	//---------------------------------------------------------------------------------------------
		std::string GetCurrentTimeAsString();
	 std::string GetCurrentTimeAsStringWithBrackets();
	 void InitConsole();
	 void InitLogFile();
		void Initialize();
	void Exit();
	void OpenFile();
	void Info( FString s);
	void Info( const char* s);
	void Error(FString s);
	void Error(const char* s);
	void Warning(FString s);
	void Warning(const char* s);
	TArray<std::pair<int, FString>>& GetData();
private:
	TArray<std::pair<int, FString>> logData;

//  	VARIADIC_LOG_FN(Error)
//  	VARIADIC_LOG_FN(Warning)
//  	VARIADIC_LOG_FN(Info)
};
#define  Log_Info(s)  Log::Get()->Info(s);
#define  Log_Error(s)  Log::Get()->Error(s);
#define  Log_Warning(s)  Log::Get()->Warning(s);

