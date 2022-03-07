#pragma once
#include "..\..\..\Core\UI\Widget.h"



class UDearImgui :public UWidget
{
public:
	typedef UWidget Super;
	UDearImgui();
	virtual ~UDearImgui();
public:
	virtual void Init();
	virtual void BeginPaly()override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(float DeltaSeconds)override;
	virtual void Destroy()override;
private:
	//���log������־
	void DrawOutputLog();
};