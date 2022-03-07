#include "../../../../Public/Core/Object/Actor/Actor.h"
#include "../../../../Public/Core/Object/Object/Component/SceneComponent.h"
#include "../../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../Core/FunctionLibrary/GamePlayStatics.h"


AActor::AActor()
{

	U_RootComponent = new USceneComponent();
}

AActor::~AActor()
{
	if (U_RootComponent)
	{
		TArray<USceneComponent*>& chlid = U_RootComponent->GetAttachChildComponent();
		for (size_t i = 0; i < chlid.size(); i++)
		{
			if (chlid[i])
			{
				delete chlid[i];
				chlid[i] = nullptr;
			}
		}
		delete U_RootComponent;
		U_RootComponent = nullptr;
	}
}

void AActor::Init()
{
	if (!U_RootComponent)
	{
		U_RootComponent = new  USceneComponent();
	}
}

void AActor::BeginPaly()
{
	U_RootComponent->BeginPlay();
}

void AActor::Tick(float DeltaSeconds)
{
	U_RootComponent->Tick(DeltaSeconds);
}

void AActor::Draw(float DeltaSeconds)
{
	U_RootComponent->Draw();
}

void AActor::Destroy()
{
	U_RootComponent->BeginDestroy();
}

void AActor::AddActorOffset(FVector DeltaLocation)
{
	U_RootComponent->AddComponentOffset(DeltaLocation);
}
				
void AActor::AddActorRotation(FRotator DeltaRotation)
{
	U_RootComponent->AddComponentRotation(DeltaRotation);
}
				
void AActor::AddActorScale3D(FVector DeltaRotation)
{
	U_RootComponent->AddComponentScale3D(DeltaRotation);
}
				
void AActor::AddActorTransform(FTransform DeltaTransform)
{
	U_RootComponent->AddComponentTransform(DeltaTransform);
}
				
void AActor::SetActorTransform(FTransform DeltaTransform)
{
	U_RootComponent->SetComponentTransform(DeltaTransform);
}
				
void AActor::SetActorLocation(FVector NewLocation)
{
	U_RootComponent->SetComponentLocation(NewLocation);
}
				
void AActor::SetActorRotation(FRotator NewRotation)
{
	U_RootComponent->SetComponentRotation(NewRotation);
}
				
void AActor::SetActorRotationAxis(EAxis::Type axis, float val)
{
	U_RootComponent->SetActorRotationAxis(axis, val);
}

void AActor::AddActorRotationAxis(EAxis::Type axis, float val)
{
	U_RootComponent->AddActorRotationAxis(axis, val);
}

void AActor::SetActorScale3D(FVector NewScale)
{
	U_RootComponent->SetComponentScale3D(NewScale);
}
				
void AActor::AddAttachComponent(USceneComponent* component)
{
	if (IsValidObject(component))
	{
		AddActorChildComponent(component);
		component->AddAttachToActor(this);
	}
	else
	{
		Log_Error("AddAttchComponent IsValidObject error");
	}
}

void AActor::ReleseAttach(class USceneComponent* component)
{
	if (component&& U_RootComponent)
	{
		TArray<USceneComponent*>& childArray = U_RootComponent->GetAttachChildComponent();
		for (TArray<USceneComponent*>::iterator it = childArray.begin(); it != childArray.end(); )
		{
			if (*it == component)
			{
				component->ReleseAttach();//释放component保存的owner
				it = childArray.erase(it); //清空Actor数组component 不能写成arr.erase(it);
				Log_Info("actor=" + GetObjectName() + " ReleseAttach component=" + component->GetObjectName());
				break;
			}
			else
			{
				++it;
			}
		}
	}
}

FVector AActor::GetActorLocation() const
{
	return U_RootComponent->GetComponentLocation();
}

FRotator AActor::GetActorRotation() const
{
	return U_RootComponent->GetComponentRotation();
}

FVector AActor::GetActorScale() const
{
	return U_RootComponent->GetComponentScale3D();
}

void AActor::AddActorChildComponent(class USceneComponent* component)
{
	if (!U_RootComponent)
	{
		U_RootComponent = component;
	}
	else
	{
		U_RootComponent->AttachChildComponent(component);
	}
}

FVector AActor::GetActorRightVector()
{
	return U_RootComponent->GetComponentRightVector();
}

FVector AActor::GetActorUpVector()
{
	return U_RootComponent->GetComponentUpVector();
}

FVector AActor::GetActorForwardVector()
{
	return U_RootComponent->GetComponentForwardVector();
}

FTransform AActor::GetActorTransform()
{
	return U_RootComponent->GetComponentTransform();
}

USceneComponent* AActor::GetActorRootComponent()
{
	return U_RootComponent;
}

bool AActor::IsVisible()
{
	return U_RootComponent->IsVisible();
}

void AActor::SetActorVisible(bool bInVisible)
{
	return U_RootComponent->SetComponentVisible(bInVisible);
}

class UWorld* AActor::GetWorld()
{
	return UGamePlayStatics::GetWorld();
}

