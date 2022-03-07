#pragma once
#include "..\Vector\String.h"
#include "..\Object\Object\Object.h"

class FFileHelper:public UObject
{
public:
	static bool LoadFileToString();
	//p1 文件夹路径   p2 赛选格式 如: ".*"  p3 返回赛选结果
	static void FindFiles(const FString& path, const FString& format,TArray<FString>& file);
};