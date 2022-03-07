#pragma once
#include "..\Object\Object\Object.h"
class UBaseWidget:public UObject
{
public:
	UBaseWidget();
	virtual ~UBaseWidget();
};
class UWidget :public UBaseWidget
{
public:
	typedef UBaseWidget Super;
	UWidget();
	virtual ~UWidget();
public:
	virtual void Init();
	virtual void BeginPaly();
	virtual void Tick(float DeltaSeconds=0.f);
	virtual void Draw(float DeltaSeconds=0.f);
	virtual void Destroy();
public:
	static UWidget* CreateWidget(UWidget* widget,unsigned int level);
};