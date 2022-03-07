#pragma once
#include "ActorComponent.h"
#include "../../../Vector/Vector.h"
#include <vector>
#include "../../../Vector/TransformVectorized.h"

class USceneComponent :public UActorComponent
{
public:
	typedef UActorComponent Super;
	USceneComponent();
	virtual ~USceneComponent();
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);
	virtual void Draw();
	virtual void BeginDestroy();
	virtual void OnRegister();
	virtual void OnUnregister();
public:
	void AddComponentOffset(FVector DeltaLocation);
	void SetComponentLocation(FVector NewLocation);
	void AddComponentRotation(FRotator DeltaRotation);
	void AddComponentRotation(FQuat DeltaRotation);
	void SetComponentRotation(FRotator NewRotation);
	void SetComponentRotation(FQuat NewRotation);
	void AddComponentScale3D(FVector DeltaScale);
	void SetComponentScale3D(FVector NewScale);
	void AddComponentTransform(FTransform DeltaTransform);
	void SetComponentTransform(FTransform DeltaTransform);
	void AttachChildComponent(USceneComponent* component);
	void SetActorRotationAxis(EAxis::Type axis, float val);
	void AddActorRotationAxis(EAxis::Type axis, float val);
	FVector GetComponentLocation() const;
	FRotator GetComponentRotation() const;
	FVector GetComponentScale3D() const;
	FVector GetComponentRightVector();
	FVector GetComponentUpVector();
	FVector GetComponentForwardVector();
	FVector GetComponentWorldLocation();
	FRotator GetComponentWorldRotation();
	FVector GetComponentWorldScale3D();
	//ÔÝÊ±ÐèÒª²¹³ä
	FTransform GetComponentWorldTransform();
	const	FTransform& GetComponentTransform();
	bool IsVisible();
	void SetComponentVisible(bool bInVisible);
	TArray<USceneComponent*>& GetAttachChildComponent();
private:
	FTransform& GetComponentTransform_Direct()const;
	bool bHiddenInGame;
	TArray<USceneComponent*> AttachChildren;
private:
	mutable	FTransform locationToWorld;

};