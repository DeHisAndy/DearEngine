#pragma once
#include "../Object/Object/Object.h"
#include <map>
#include <string>
class UKismetStringLibrary :public UObject
{
public:
	static bool GetFileSuffix(const FString& soure, FString& suffix);
};
