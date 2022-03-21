#include "../../../../../Public/Core/Object/Object/Component/MeshComponent.h"
#include "../../../Core/Render/ShaderEffect.h"
#include "../../../Core/Render/RHI.h"
#include "../../../Core/Object/Object/TextureManage.h"
#include "../../../Core/Render/RenderState.h"

UMeshComponent::UMeshComponent()
{
	RasterizerState = FRenderState::RSDefault;
	DepthStencilState = FRenderState::DSSWriteStencil;
	DepthStencilStateStencilRef = 0;
}

UMeshComponent::~UMeshComponent()
{

}

void UMeshComponent::Draw()
{
	Super::Draw();
	//设置管线渲染状态
	RenderStat();
#ifdef  SHADER_DIRECTX11_EFFECT
	for (unsigned int i = 0; i < GetMeshCout(); i++)
	{
		const	UPrimitiveComponent::IABufferInfo* vertexBuffer = GetVertexBufferDataFormIndex(i);
		const	UPrimitiveComponent::IABufferInfo* indexBuffer = GetIndexBufferDataFormIndex(i);
		if (Check(vertexBuffer) && Check(vertexBuffer))
		{
			UINT strides = vertexBuffer->strides;
			UINT offsets = 0;
			FRHI::RHI_SetVertexBuffers(0, 1, vertexBuffer->buffer.GetAddressOf(), &strides, &offsets);
			FRHI::RHI_SetIndexBuffer(indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			FRHI::RHI_SetInputLayout(GetInputLayout());
			D3DX11_TECHNIQUE_DESC techDesc;

			EffectTechnique->GetDesc(&techDesc);

			for (unsigned int i = 0; i < techDesc.Passes; i++)
			{

				ID3DX11EffectPass* pass = EffectTechnique->GetPassByIndex(i);

				pass->Apply(0, FRHI::RHI_GetID3D11DeviceContext());
				FRHI::RHI_DrawIndexed(indexBuffer->cout, 0, 0);
			}
		}

		// 		ClearSRVRender();
		// 		PresentEffect();
	}
#endif
}

void UMeshComponent::SetEffectShader(FString name, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs/*=null*/)
{
#ifdef  SHADER_DIRECTX11_EFFECT
	UShaderEffect* effect = dynamic_cast<UShaderEffect*>(FRHI::RHI_GetShaderCompilerWorker());
	if (effect)
	{

		auto _struct = effect->GetData().find(name.GetString());
		if (_struct != effect->GetData().end())
		{
			HR(_struct->second->Effect->CloneEffect(D3DX11_EFFECT_CLONE_FORCE_NONSINGLE, Effect.GetAddressOf()));
			Log_Info(GetObjectName() + "  Mesh Init Material=" + name);
			EffectTechnique = Effect->GetTechniqueByName("BasicTech");
			D3DX11_PASS_DESC PassDesc;
			HR(EffectTechnique->GetPassByIndex(0)->GetDesc(&PassDesc));
			if (pInputElementDescs == nullptr)
			{
				HR(FRHI::RHI_GetD3d11Device()->CreateInputLayout(UShaderCompilerWorker::Shader_InputInfo::Inputlayout, ARRAYSIZE(UShaderCompilerWorker::Shader_InputInfo::Inputlayout), PassDesc.pIAInputSignature,
					PassDesc.IAInputSignatureSize, GetInputLayoutAddress()));
			}
			else
			{
				//自定义顶点输入结构体

			}

			Log_Info("Init Vetex Struct InputLayout");
		}
		else
		{
			Log_Error("effectStruct not Find " + name);
		}
	}
	else
	{
		Log_Error("FRHI::GetShaderCompilerWorker() dynamic_cast UShaderEffect error");
	}
	Super::SetEffectShader(name);

#endif
}


void UMeshComponent::SetVectorParameterValue(const FString& shaderVariableName, FVector4D& vector)
{
#ifdef  SHADER_DIRECTX11_EFFECT
	Effect->GetVariableByName(shaderVariableName.GetString().c_str())->AsVector()->SetFloatVector(reinterpret_cast<float*>(&vector));
#endif
}

void UMeshComponent::SetBoolParameterValue(const FString& shaderVariableName, bool value) 
{
#ifdef  SHADER_DIRECTX11_EFFECT
	Effect->GetVariableByName(shaderVariableName.GetString().c_str())->AsScalar()->SetBool(value);
#endif
}

void UMeshComponent::SetMaterixParameterValue(const FString& shaderVariableName, FMatrix& matrix)
{
#ifdef  SHADER_DIRECTX11_EFFECT
	Effect->GetVariableByName(shaderVariableName.GetString().c_str())->AsMatrix()->SetMatrix(reinterpret_cast<float*>(&matrix));
#endif
}

void UMeshComponent::SetScalarParameterValue(const FString& shaderVariableName, float value)
{
#ifdef  SHADER_DIRECTX11_EFFECT
	Effect->GetVariableByName(shaderVariableName.GetString().c_str())->AsScalar()->SetFloat(value);
#endif
}

void UMeshComponent::SetTextureParameterValue(const FString& shaderVariableName, const Texture* texture)
{
#ifdef  SHADER_DIRECTX11_EFFECT
	if (texture && texture->_srv)
	{
		Effect->GetVariableByName(shaderVariableName.GetString().c_str())->AsShaderResource()->SetResource(texture->_srv);
	}
	else
	{
		Log_Error("SetTextureParameterValue texture==null or srv==null");
	}
#endif
}

void UMeshComponent::SetSamplerParameterValue(const FString& shaderVariableName, ID3D11SamplerState* sampler)
{
#ifdef  SHADER_DIRECTX11_EFFECT
	if (sampler)
	{
		Effect->GetVariableByName(shaderVariableName.GetString().c_str())->AsSampler()->SetSampler(0, sampler);
	}
	else
	{
		Log_Error("SetSamplerParameterValue sampler==null ");
	}
#endif
}

void UMeshComponent::SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef /*= 0*/)
{
	DepthStencilState = pDepthStencilState;
	DepthStencilStateStencilRef = StencilRef;
	
}

void UMeshComponent::SetRasterizerState(ID3D11RasterizerState* pRasterizerState)
{
	RasterizerState = pRasterizerState;	
}

void UMeshComponent::RenderStat()
{
	FRHI::RHI_GetID3D11DeviceContext()->OMSetDepthStencilState(DepthStencilState.Get(), DepthStencilStateStencilRef);
	FRHI::RHI_GetID3D11DeviceContext()->RSSetState(RasterizerState.Get());
}
