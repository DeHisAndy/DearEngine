#include <d3dcompiler.h>
#include "..\..\..\Core\Render\ShaderEffect.h"
#include "..\..\..\Core\FunctionLibrary\KismetStringLibrary.h"
#include "..\..\..\Core\FunctionLibrary\KismetSystemLibrary.h"
#include "..\..\..\Core\Render\RHI.h"
#include "..\..\..\Core\Helper\FileHelper.h"



UShaderEffect::UShaderEffect()
{

}

UShaderEffect::~UShaderEffect()
{

}

std::map<FString, UShaderEffect::EffectSturct*>& UShaderEffect::GetData()
{
	return effectStruct;
}

void UShaderEffect::Init()
{
	Super::Init();
}

void UShaderEffect::CompileHlSL()
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
	// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
		// 但仍然允许着色器进行优化操作
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// 在Debug环境下禁用优化以避免出现一些不合理的情况
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	TArray<FString> Teamp;
	FFileHelper::FindFiles("Assets/Shaders/Public", ".usf", Teamp);
	for (int i = 0; i < Teamp.size(); i++)
	{
		EffectSturct* effectShader = new EffectSturct();
		//直接加载
		// Message contains "This is a big test"
		FString xx = Teamp[i];
		ComPtr<ID3DBlob> error;
		HR(D3DX11CompileEffectFromFile(*xx, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, dwShaderFlags, 0, FRHI::RHI_GetD3d11Device(), &effectShader->Effect, &error));
		Log_Info("Compile shader = " + Teamp[i]);
		//加载缓存
// 			{
// 				std::string cso_Path = "Assets/ShaderCSO/" + iter->first;
// 				ComPtr<ID3DBlob> blob;
// 				FMath::CreateShaderFromFile(FMath::StringConvertWstring(cso_Path).c_str(), FMath::StringConvertWstring(iter->second + iter->first).c_str(), "FX", "fx_5_0", blob.ReleaseAndGetAddressOf());
// 				D3DX11CreateEffectFromMemory(blob->GetBufferPointer(), blob->GetBufferSize(), 0, BaseGameThread::Get()->GetD3DInterface().D3D11Device.Get(), &1->Effect);
// 			}
		this->AddShaderName(FString(Teamp[i]));
		effectStruct.insert_or_assign(Teamp[i], effectShader);
	} 
	Super::CompileHlSL();
}

