#pragma once
#include "../Object.h"
class UGameInstance:public UObject
{
public:
	typedef UObject Super;

	UGameInstance();
	UGameInstance(class UWorld* world,class UShaderCompilerWorker* effect=nullptr,class UGameViewport* gameViewport=nullptr);
	virtual~UGameInstance();
	virtual void Init();
	virtual void BeginPaly();
	virtual void UpdateScene(float DeltaSeconds);
	virtual void DrawScene(float DeltaSeconds);
	virtual void Destroy();
	virtual void OnResize();
public:
	void SetWorld(class UWorld* world);
	void SetEffect(class UShaderCompilerWorker* effect);
private:
	void ImguiInit();
private:
	//������ػ���
	void EngineLoadingPar();
	//����������ʼ��content��������Դ
	void LoadEngineContentAssetsResource();

public:
	class	UWorld* GetWorld();
	class	UShaderCompilerWorker* GetEffect();
	class   UGameViewport* GetGameViewport();
private:
	class	UWorld* defaultWrold;
	class   UShaderCompilerWorker* defulatShaderEffect;
	class   UGameViewport* defulatGameViewport;
};