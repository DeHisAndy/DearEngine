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
	// ���� D3DCOMPILE_DEBUG ��־���ڻ�ȡ��ɫ��������Ϣ���ñ�־���������������飬
		// ����Ȼ������ɫ�������Ż�����
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// ��Debug�����½����Ż��Ա������һЩ����������
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	TArray<FString> Teamp;
	FFileHelper::FindFiles("Assets/Shaders/Public", ".usf", Teamp);
	for (int i = 0; i < Teamp.size(); i++)
	{
		EffectSturct* effectShader = new EffectSturct();
		//ֱ�Ӽ���
		// Message contains "This is a big test"
		FString xx = Teamp[i];
		ComPtr<ID3DBlob> error;
		HR(D3DX11CompileEffectFromFile(*xx, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, dwShaderFlags, 0, FRHI::RHI_GetD3d11Device(), &effectShader->Effect, &error));
		Log_Info("Compile shader = " + Teamp[i]);
		//���ػ���
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

