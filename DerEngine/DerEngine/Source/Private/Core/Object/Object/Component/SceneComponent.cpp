#include "../../../../../Public/Core/Object/Object/Component/SceneComponent.h"
#include "../../../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../Core/Object/Actor/Actor.h"

USceneComponent::USceneComponent()
{
}

USceneComponent::~USceneComponent()
{

}

void USceneComponent::BeginPlay()
{
	for (size_t i = 0; i < AttachChildren.size(); i++)
	{
		if (IsValidObject(AttachChildren[i]))
		{
			AttachChildren[i]->BeginPlay();
		}
		else
		{
			Log_Error("IsValidObject：AttachChildren index=" + FString::FormatIntToString(i));
		}
	}

}

void USceneComponent::Tick(float DeltaSeconds)
{
	for (size_t i = 0; i < AttachChildren.size(); i++)
	{
		if (IsValidObject(AttachChildren[i]))
		{
			AttachChildren[i]->Tick(DeltaSeconds);
		}
		else
		{
			Log_Error("IsValidObject：AttachChildren index=" + FString::FormatIntToString(i));
		}
	}

}

void USceneComponent::Draw()
{
	for (size_t i = 0; i < AttachChildren.size(); i++)
	{
		if (IsValidObject(AttachChildren[i]))
		{
			AttachChildren[i]->Draw();
		}
		else
		{
			Log_Error("IsValidObject：AttachChildren index=" + FString::FormatIntToString(i));
		}
	}

}

void USceneComponent::BeginDestroy()
{
	for (size_t i = 0; i < AttachChildren.size(); i++)
	{
		if (IsValidObject(AttachChildren[i]))
		{
			AttachChildren[i]->BeginDestroy();
		}
		else
		{
			Log_Error("IsValidObject：AttachChildren index=" + FString::FormatIntToString(i));
		}
	}
}

void USceneComponent::OnRegister()
{

}

void USceneComponent::OnUnregister()
{

}


void USceneComponent::AddComponentOffset(FVector DeltaLocation)
{
	const FVector NewWorldLocation = DeltaLocation + GetComponentTransform().GetTranslation();
	SetComponentLocation(NewWorldLocation);
}

void USceneComponent::SetComponentLocation(FVector NewLocation)
{
	locationToWorld.SetTranslation(NewLocation);
}

void USceneComponent::AddComponentRotation(FRotator DeltaRotation)
{
	const FQuat NewWorldRotation = GetComponentTransform().GetRotation()*DeltaRotation.Quaternion();
	SetComponentRotation(NewWorldRotation);
}

void USceneComponent::AddComponentRotation(FQuat DeltaRotation)
{
	const FQuat NewWorldRotation = DeltaRotation * GetComponentTransform().GetRotation();
	SetComponentRotation(NewWorldRotation);
}

void USceneComponent::AddComponentScale3D(FVector DeltaScale)
{
	FTransform& transform = GetComponentTransform_Direct();
	SetComponentScale3D(transform.GetScale3D() + DeltaScale);
}

void USceneComponent::AddComponentTransform(FTransform DeltaTransform)
{
	FTransform& transform = GetComponentTransform_Direct();
	AddComponentOffset(DeltaTransform.GetLocation());
	AddComponentRotation(DeltaTransform.GetRotation());
	AddComponentScale3D(DeltaTransform.GetScale3D());
}

void USceneComponent::SetComponentTransform(FTransform DeltaTransform)
{
	FTransform& transform = GetComponentTransform_Direct();
	transform = DeltaTransform;
}


void USceneComponent::SetComponentRotation(FRotator NewRotation)
{
//	if (!NewRotation.Equals(GetComponentRotation(), KINDA_SMALL_NUMBER))
	{
		FTransform& transform = GetComponentTransform_Direct();
		transform.SetRotation(NewRotation.Quaternion());
	}
}


void USceneComponent::SetComponentRotation(FQuat NewRotation)
{
	FTransform& transform = GetComponentTransform_Direct();
	//if (!NewRotation.Equals(transform.GetRotation(), KINDA_SMALL_NUMBER))
	{
		transform.SetRotation(NewRotation);
	}
}

void USceneComponent::SetComponentScale3D(FVector NewScale)
{
	GetComponentTransform_Direct().SetScale3D(NewScale);
}

void USceneComponent::AttachChildComponent(USceneComponent* component)
{
	if (IsValidObject(component))
	{
		bool cout = false;
		for (size_t i = 0; i < AttachChildren.size(); i++)
		{
			if (AttachChildren[i]==component)
			{
				cout = true;
				break;
			}
		}
		if (!cout)
		{
			AttachChildren.push_back(component);
		}
		else
		{
			Log_Error("USceneComponent=" + GetObjectName() + " already existed the component=" + component->GetObjectName());
		}

	}
}

void USceneComponent::SetActorRotationAxis(EAxis::Type axis, float val)
{
	switch (axis)
	{
	case EAxis::None:
		break;
	case EAxis::X:
		SetComponentRotation(FRotator(val, 0, 0));
		break;
	case EAxis::Y:
		SetComponentRotation(FRotator(0, val, 0));
		break;
	case EAxis::Z:
		SetComponentRotation(FRotator(0, 0, val));
		break;
	default:
		break;
	}
}

void USceneComponent::AddActorRotationAxis(EAxis::Type axis, float val)
{
// 	switch (axis)
// 	{
// 	case EAxis::None:
// 		break;
// 	case EAxis::X:
// 		AddComponentRotation(FRotator(val, 0, 0));
// 		break;
// 	case EAxis::Y:
// 		AddComponentRotation(FRotator(0, val, 0));
// 		break;
// 	case EAxis::Z:
// 		AddComponentRotation(FRotator(0, 0, val));
// 		break;
// 	default:
// 		break;
// 	}
}

FVector USceneComponent::GetComponentLocation() const
{
	return GetComponentTransform_Direct().GetTranslation();
}

FRotator USceneComponent::GetComponentRotation() const
{
	return GetComponentTransform_Direct().Rotator();
}

FVector USceneComponent::GetComponentScale3D() const
{
	return GetComponentTransform_Direct().GetScale3D();
}

FVector USceneComponent::GetComponentRightVector()
{
	return locationToWorld.GetUnitAxis(EAxis::X);
}

FVector USceneComponent::GetComponentUpVector()
{
	return locationToWorld.GetUnitAxis(EAxis::Y);
}

FVector USceneComponent::GetComponentForwardVector()
{
	return locationToWorld.GetUnitAxis(EAxis::Z);
}

FVector USceneComponent::GetComponentWorldLocation()
{
	AActor* owner = GetOwner();
	//如果绑定actor 则Actor+component
	if (owner)
	{
		return owner->GetActorLocation() + GetComponentLocation();
	}
	return GetComponentLocation();
}

FRotator USceneComponent::GetComponentWorldRotation()
{
	AActor* owner = GetOwner();
	//如果绑定actor 则Actor+component
	if (owner)
	{
		return owner->GetActorRotation() + GetComponentRotation();
	}
	return GetComponentRotation();
}

FVector USceneComponent::GetComponentWorldScale3D()
{
	AActor* owner = GetOwner();
	//如果绑定actor 则Actor+component
	if (owner)
	{
		return owner->GetActorScale() * GetComponentScale3D();
	}
	return GetComponentScale3D();
}

FTransform USceneComponent::GetComponentWorldTransform()
{
	AActor* owner = GetOwner();
	//如果绑定actor 则Actor+component
	if (owner)
	{
		
    	return 	FTransform(owner->GetActorRotation()+GetComponentRotation(),owner->GetActorLocation()+GetComponentLocation(),owner->GetActorScale()*GetComponentScale3D());
	}
	return GetComponentTransform();
}

const FTransform& USceneComponent::GetComponentTransform()
{
	return locationToWorld;
}

bool USceneComponent::IsVisible()
{
	return bHiddenInGame;
}

void USceneComponent::SetComponentVisible(bool bInVisible)
{

}

std::vector<USceneComponent*>& USceneComponent::GetAttachChildComponent()
{
	return AttachChildren;
}

FTransform& USceneComponent::GetComponentTransform_Direct() const
{
	return locationToWorld;
}
