#include "..\..\..\Core\Object\Object\Component\Light\PointLightComponent.h"

UPointLightComponent::UPointLightComponent()
{

}

UPointLightComponent::~UPointLightComponent()
{

}

FVector4D UPointLightComponent::GetLightColor()
{
	return lightColor;
}

