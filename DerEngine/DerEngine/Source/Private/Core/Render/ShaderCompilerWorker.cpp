#include "../../../Public/Core/Vector/String.h"
#include "../../../Public/Core/Helper/Log.h"
#include "../../../Public/Core/Render/ShaderCompilerWorker.h"
D3D11_INPUT_ELEMENT_DESC UShaderCompilerWorker::Shader_InputInfo::Inputlayout[5] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
UShaderCompilerWorker::UShaderCompilerWorker()
{

}

UShaderCompilerWorker::~UShaderCompilerWorker()
{

}

void UShaderCompilerWorker::Init()
{
	Log_Info(FString("Effect Init"));
	CompileHlSL();
}

void UShaderCompilerWorker::CompileHlSL()
{
	Log_Info(FString("CompileHlSL of Number=")+FString(FString::FormatIntToString((int)shaderName.size())));
}

TArray<class FString>& UShaderCompilerWorker::GetShaderArry()
{
	return shaderName;
}

void UShaderCompilerWorker::AddShaderName(class FString pathName)
{
	shaderName.push_back(pathName);
}

void UShaderCompilerWorker::RemoveShaderName(class FString pathName)
{
	for (size_t i = 0; i < shaderName.size(); i++)
	{
		if (shaderName[i]== pathName)
		{
			shaderName.erase(shaderName.begin() + i);
		}
	}
}

void UShaderCompilerWorker::ClearShaderName()
{
	shaderName.clear();
}
