#pragma once
#include "../Component/ActorComponent.h"
#include "../../Actor/Actor.h"
class UWorld:public UObject
{
public:
	typedef UWorld Super;
	UWorld();
	UWorld(class AGameMode* GameMode);
	virtual ~UWorld();
	virtual void Init();
	virtual void BeginPaly();
	virtual void UpdateScene(float DeltaSeconds);
	virtual void DrawScene(float DeltaSeconds);
	virtual void Destroy();
	virtual void AddActorToWorld(AActor* Actor);
protected:
	//初始化场景时候加载
	virtual	void PostCreateWorld();
	//场景初始化完 创建场景组件
	virtual	void LoadWorld();
public:
	class	AGameMode* GetGameMode();
	void SetGameMode(class AGameMode* GameMode);
public:
	template<typename T>
	 AActor* SpawnActor()
	{
		 AActor* type = new T();
		 if (type)
		 {
			 AddActorToWorld(type);
		 }
		 return type;
	}
	 template<typename T>
	 AActor* SpawnComponent()
	 {
		 AActor* actor = new AActor();
		 UActorComponent* type = new T();
		 if (actor&&type)
		 {
			 actor->AddAttachComponent(type);
			 AddActorToWorld(actor);
		 }
		 return type;
	 }
private:
	class AGameMode* A_Gamemode;
	TArray<class AActor*> arrayActor;
};