#include "../../../../../Public/Core/Object/Object/Component/ActorComponent.h"
#include "../../../../../Public/Core/Object/Object/World/World.h"
#include "../../../../../Public/Core/FunctionLibrary/GamePlayStatics.h"
#include "../../../../../Public/Core/Object/Object/GameInstance/GameInstance.h"


UActorComponent::UActorComponent()
{
	owner = nullptr;
}

UActorComponent::~UActorComponent()
{

}

UWorld* UActorComponent::GetWorld()
{
	return UGamePlayStatics::GetGameInstance()->GetWorld();
}

void UActorComponent::AddAttachToActor(class AActor* actor)
{
	if (actor)
	{
		owner = actor;
	}
	else
	{
		Log_Info("UActorComponent=" + GetObjectName() + " SetOwner error");
	}
}

void UActorComponent::ReleseAttach()
{
	owner = nullptr;
}

class AActor* UActorComponent::GetOwner()
{
	return owner;
}
