#pragma once
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Object/Object/World/World.h"
class UDeferredRenderingWorld :public UWorld
{
public:
	typedef UWorld Super;
	UDeferredRenderingWorld();
	UDeferredRenderingWorld(class AGameMode* gameMode);
	virtual ~UDeferredRenderingWorld();
	virtual void Init();
	virtual void BeginPaly();
	virtual void UpdateScene(float DeltaSeconds);
	virtual void DrawScene(float DeltaSeconds);
	virtual void Destroy();
protected:
	//初始化场景时候加载
	virtual	void PostCreateWorld() override;
	//场景初始化完 创建场景组件
	virtual	void LoadWorld()override;

};