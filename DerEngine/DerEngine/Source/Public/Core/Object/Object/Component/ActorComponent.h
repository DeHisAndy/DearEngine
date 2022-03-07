#pragma once
#include "../Object.h"

class UActorComponent : public UObject
{
public:
	typedef UObject Super;
	UActorComponent();
	virtual ~UActorComponent();
	virtual void BeginPlay() {};
	virtual void Tick(float DeltaSeconds) {};
	virtual void Draw(float DeltaSeconds) {};
	virtual void BeginDestroy() {};
	virtual void OnRegister() {};
	virtual void OnUnregister() {};
public:
	class UWorld* GetWorld();
	void AddAttachToActor(class AActor* actor);
	void ReleseAttach();
	class AActor* GetOwner();
private:
	class AActor* owner;
};