#pragma once
#include "LightComponent.h"
class UPointLightComponent : public ULightComponent
{
public:
	typedef ULightComponent Super;
	UPointLightComponent();
	virtual ~UPointLightComponent();
public:
	virtual FVector4D GetLightColor();
};