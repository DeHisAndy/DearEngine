#pragma once
#include "..\Vector\String.h"
#include "..\Object\Object\Object.h"

class FFileHelper:public UObject
{
public:
	static bool LoadFileToString();
	//p1 �ļ���·��   p2 ��ѡ��ʽ ��: ".*"  p3 ������ѡ���
	static void FindFiles(const FString& path, const FString& format,TArray<FString>& file);
};