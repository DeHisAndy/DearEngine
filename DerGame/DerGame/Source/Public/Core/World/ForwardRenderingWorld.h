#pragma once
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Object/Object/World/World.h"
class UForwardRenderingWorld :public UWorld
{
public:
	typedef UWorld Super;
	UForwardRenderingWorld();
	UForwardRenderingWorld(class AGameMode* gameMode);
	virtual ~UForwardRenderingWorld();
	virtual void Init();
	virtual void BeginPaly();
	virtual void UpdateScene(float DeltaSeconds);
	virtual void DrawScene(float DeltaSeconds);
	virtual void Destroy();
private:
	//初始化场景组件
	void InitWorld();

};