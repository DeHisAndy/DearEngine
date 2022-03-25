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
	//��ʼ������ʱ�����
	virtual	void PostCreateWorld() override;
	//������ʼ���� �����������
	virtual	void LoadWorld()override;

};