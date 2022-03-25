#pragma once
#include <vector>

#include <map>
#include <functional>
#include "..\..\Vector\String.h"
template< class T >
T* NewObject()
{
	return new T;
}
template<typename CharT>
using TArray = std::vector<CharT>;

template<typename CharT,typename CharV>
using TMap= std::map<CharT, CharV>;
template<typename CharT, typename CharV>
using TMulMap= std::multimap<CharT, CharV>;

typedef std::function<void()> taskEvent;

class UObject
{
public:
	UObject();
	virtual ~UObject();

public:
	FString GetObjectName();
	FString GetClassName();
	FString GetPathName();
	virtual void SetObjectName(const FString& str);
	void SetPathName(const FString& str);
	void SetClassName(const FString& name);
private:
	void InitResource();
protected:
	FString objectName;
	FString className;
	FString pathName;
	static TArray<UObject*> hashObject;
	TArray<FString> inheritClassList;
};