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
	// 检测 MSAA支持的质量等级
	unsigned int m_4xMsaaQuality;
	bool F_RenderStart;
};
