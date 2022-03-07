#include "../../../../Public/Core/Object/Object/Object.h"
#include <iostream>
#include <typeinfo>
#include "../../../../Public/Core/Helper/Log.h"
TArray<UObject*> UObject::hashObject = TArray<UObject*>();
UObject::UObject()
{
    //将指针添加到统一容器
	UObject::hashObject.push_back(this);

}

UObject::~UObject()
{


}


void UObject::InitResource()
{
	if (objectName.Empty() || className.Empty())
	{
		for (size_t i = 0; i < UObject::hashObject.size(); i++)
		{
			if (UObject::hashObject[i] == this)
			{
				if (i == 0)
				{
					objectName = FString(typeid(*this).name());
					SetClassName(objectName);
				}
				else
				{
					objectName = FString(typeid(*this).name()) + "_" + FString::FormatIntToString(i);
					SetClassName(FString(typeid(*this).name()));
					TArray<FString> out;
					FString::Split(objectName, " ", out);
					if (out.size() > 1)
					{
						objectName = out[1];
					}
				}
			}
		}
	}
}


FString UObject::GetObjectName()
{
	InitResource();
	return objectName;
}

FString UObject::GetClassName()
{
	InitResource();
	return className;
}

FString UObject::GetPathName()
{
	return pathName;

}

void UObject::SetObjectName(const FString& str)
{
	objectName = str;
}

void UObject::SetPathName(const FString& str)
{
	pathName = str;
}

void UObject::SetClassName(const FString& name)
{
	className = name;
}

