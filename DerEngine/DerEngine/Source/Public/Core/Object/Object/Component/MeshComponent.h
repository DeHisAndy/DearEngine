#include "PrimitiveComponent.h"
//封装shader反射框架 使用的是Directx11 Effect
#define SHADER_DIRECTX11_EFFECT

#ifdef  SHADER_DIRECTX11_EFFECT
#include <d3dx11effect.h>
#endif

class UMeshComponent :public UPrimitiveComponent
{

public:
	typedef UPrimitiveComponent Super;
	UMeshComponent();
	virtual ~UMeshComponent();
public:
	virtual void Draw();
public:
	virtual void SetEffectShader(FString name, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs = nullptr);
public:
	virtual	void SetVectorParameterValue(const FString& shaderVariableName, FVector4D& vector)override;
	virtual	void SetBoolParameterValue(const FString& shaderVariableName, bool value) override;
	virtual	void SetMaterixParameterValue(const FString& shaderVariableName, FMatrix& matrix)override;
	virtual	void SetScalarParameterValue(const FString& shaderVariableName, float value)override;
	virtual	void SetTextureParameterValue(const FString& shaderVariableName, const Texture* texture)override;
	virtual	void SetSamplerParameterValue(const FString& shaderVariableName, ID3D11SamplerState* sampler)override;
protected:
#ifdef  SHADER_DIRECTX11_EFFECT
	ComPtr < ID3DX11Effect> Effect;
	ComPtr < ID3DX11EffectTechnique> EffectTechnique;
#endif

};