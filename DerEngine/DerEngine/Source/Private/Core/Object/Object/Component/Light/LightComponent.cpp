#include "..\..\..\Core\Object\Object\Component\Light\LightComponent.h"

ULightComponent::ULightComponent()
{
	lightColor = FVector4D(1.f, 1.f, 1.f, 1.f);
	lightRadius = 1000.f;
}

ULightComponent::~ULightComponent()
{

}

FVector4D ULightComponent::GetLightColor()
{
	return lightColor;
}

float ULightComponent::GetLightRadius()
{
	return lightRadius;
}

void ULightComponent::SetLightColor(const FVector4D& color)
{
	lightColor = color;
}

void ULightComponent::SetLightRadius(float value)
{
	lightRadius = FMath::Clamp<float>(value,0,9999);
}
