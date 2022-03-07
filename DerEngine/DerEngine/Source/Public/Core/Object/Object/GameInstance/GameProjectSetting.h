#pragma once
#include "../Object.h"
class UGameProjectSetting :public UObject
{
public:
	typedef UObject Super;
	UGameProjectSetting();
	virtual~UGameProjectSetting();
public:
	virtual void initializationProjectSetting();
	void SetGameFontDir(FString dir);
	FString GetGameFontDir();
private:
	FString fontPath;
};