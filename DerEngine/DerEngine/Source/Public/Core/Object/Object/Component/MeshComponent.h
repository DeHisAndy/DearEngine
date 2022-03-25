#include "PrimitiveComponent.h"
#include "../../../Engine/CoreTypes.h"

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
	virtual	void SetMaterixParameterValue(const FString& shaderVariableName,float* matrix)override;
	virtual	void SetScalarParameterValue(const FString& shaderVariableName, float value)override;
	virtual	void SetTextureParameterValue(const FString& shaderVariableName, const Texture* texture)override;
	virtual	void SetTextureParameterValue(const FString& shaderVariableName, ID3D11ShaderResourceView* texture)override;
	virtual	void SetSamplerParameterValue(const FString& shaderVariableName, ID3D11SamplerState* sampler)override;
	virtual void SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef = 0)override;
	virtual void SetRasterizerState(ID3D11RasterizerState* pRasterizerState)override;
	virtual void RenderStat();
protected:
#ifdef  SHADER_DIRECTX11_EFFECT
	ComPtr < ID3DX11Effect> Effect;
	ComPtr < ID3DX11EffectTechnique> EffectTechnique;
	ComPtr<ID3D11RasterizerState> RasterizerState;
	ComPtr<ID3D11DepthStencilState> DepthStencilState;
	int DepthStencilStateStencilRef;
#endif

};