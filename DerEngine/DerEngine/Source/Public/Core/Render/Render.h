#pragma once
#include "RenderBase.h"

class FRender :public FRenderBase
{
public:
	FRender();
	virtual~FRender();
public:
	virtual void Init();
	virtual void BeginPaly();
	virtual void OnResize();
	virtual void UpdateScene(float DeltaSeconds);
	virtual void Draw(float DeltaSeconds);
	virtual void Destroy();
public:
	void InitDirect3D();
	bool GetRenderStart();
private:
	bool m_Enable4xMsaa;
	// ��� MSAA֧�ֵ������ȼ�
	unsigned int m_4xMsaaQuality;
	bool F_RenderStart;
};
