#pragma once
#include "../Object/Object.h"
#include "../../Vector/Vector.h"
#include "../../Vector/TransformVectorized.h"
#include "../../Vector/MathType.h"
class AActor : public UObject
{
public:
	typedef UObject Super;
	virtual void Init();
	virtual void BeginPaly();
	virtual void Tick(float DeltaSeconds) ;
	virtual void Draw();
	virtual void Destroy();
public:
	static AActor* SpawnActor();
public:
	void AddActorOffset(FVector DeltaLocation);
	void AddActorRotation(FRotator DeltaRotation);
	void AddActorScale3D(FVector DeltaRotation);
	void AddActorTransform(FTransform DeltaTransform);
	void SetActorTransform(FTransform DeltaTransform);
	void SetActorLocation(FVector NewLocation);
	void SetActorRotation(FRotator NewRotation);
	void SetActorRotationAxis(EAxis::Type axis, float val);
	void AddActorRotationAxis(EAxis::Type axis, float val);
	void SetActorScale3D(FVector NewScale);
	void AddAttachComponent(class USceneComponent* component);
	void ReleseAttach(class USceneComponent* component);
	FVector GetActorLocation() const;
	FRotator GetActorRotation() const;
	FVector GetActorScale() const;
	FVector GetActorRightVector();
	FVector GetActorUpVector();
	FVector GetActorForwardVector();
    FTransform GetActorTransform();
	class USceneComponent* GetActorRootComponent();
	bool IsVisible();
	void SetActorVisible(bool bInVisible);
	static class UWorld* GetWorld();
private:
	void AddActorChildComponent(class USceneComponent* component);
public:
	AActor();
	virtual ~AActor();
private:
	class USceneComponent* U_RootComponent;
};