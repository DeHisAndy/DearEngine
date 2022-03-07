#pragma once
#include "MeshComponent.h"
class UAssimpPrimitiveComponent :public UMeshComponent
{
public:
	typedef UMeshComponent Super;
	UAssimpPrimitiveComponent();
	UAssimpPrimitiveComponent(TArray<FMeshPrimitiveComponent>& arrayData, FString shaderName = FString());
	UAssimpPrimitiveComponent(FString meshPath, FString shaderName = FString());
	virtual ~UAssimpPrimitiveComponent();
public:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);
	virtual void Draw(float DeltaSeconds);
	virtual void BeginDestroy();
	virtual void OnRegister();
	virtual void OnUnregister();
private:
	virtual void ImportModel(const  FString& modelPath);
public:
	void Draw();
};