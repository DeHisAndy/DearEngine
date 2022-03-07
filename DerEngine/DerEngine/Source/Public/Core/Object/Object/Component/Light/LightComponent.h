#pragma once
#include "..\SceneComponent.h"

class ULightComponent : public USceneComponent
{
public:
	typedef USceneComponent Super;
	ULightComponent();
	virtual ~ULightComponent();
public:
	 FVector4D GetLightColor();
	 float GetLightRadius();
	 void SetLightColor(const FVector4D& color);
	 void SetLightRadius(float value);
protected:
	FVector4D lightColor;
	float lightRadius;
};