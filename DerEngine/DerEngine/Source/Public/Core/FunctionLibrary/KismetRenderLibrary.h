#pragma once

#include <d3d11.h>
#include "..\Object\Object\Component\PrimitiveComponent.h"
#include "..\Object\Object\Object.h"

class UKismetRenderLibrary:public UObject
{
public:
	static void RHIDX11_DrawMatrialToRenderTarget(ID3D11RenderTargetView* ppRenderTargetViews, const FString& matrial,const FString& texture2D, class UPrimitiveComponent* mesh);
	static bool CheckShaderValid(const FString& matrialName);
	static void CreateSquare(TArray<UPrimitiveComponent::FMeshPrimitiveComponent>& arrayData);
	static class AActor* CreateSquare(const FString& matrial);
private:

};
