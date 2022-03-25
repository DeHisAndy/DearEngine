#include "../../../../../Public/Core/Object/Object/Component/PrimitiveComponent.h"
#include "../../../../../Public/Core/Render/ShaderCompilerWorker.h"
#include "../../../../../Public/Core/Vector/String.h"
#include "../../../../../Public/Core/Helper/Log.h"
#include "../../../../../Public/Core/Render/RHI.h"
#include "../../../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"

std::multimap<FString, UPrimitiveComponent*> UPrimitiveComponent::proyxRenderData = std::multimap<FString, UPrimitiveComponent*>();

std::multimap<FString, UPrimitiveComponent*>& UPrimitiveComponent::GetProyxRenderData()
{
	return	UPrimitiveComponent::proyxRenderData;
}

UPrimitiveComponent::UPrimitiveComponent()
{

}

UPrimitiveComponent::~UPrimitiveComponent()
{
}

void UPrimitiveComponent::BeginPlay()
{

	Super::BeginPlay();
}

void UPrimitiveComponent::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void UPrimitiveComponent::Draw()
{
	Super::Draw();

}

void UPrimitiveComponent::BeginDestroy()
{
	Super::BeginDestroy();
}

void UPrimitiveComponent::OnRegister()
{
	Super::OnRegister();
}

void UPrimitiveComponent::OnUnregister()
{
	Super::OnUnregister();
}

void UPrimitiveComponent::SetVectorParameterValue(const FString& shaderVariableName,  FVector4D& vector)
{

}

void UPrimitiveComponent::SetBoolParameterValue(const FString& shaderVariableName, bool value)
{

}

void UPrimitiveComponent::SetMaterixParameterValue(const FString& shaderVariableName,  FMatrix& matrix)
{

}


void UPrimitiveComponent::SetMaterixParameterValue(const FString& shaderVariableName, float* matrix)
{

}

void UPrimitiveComponent::SetScalarParameterValue(const FString& shaderVariableName, float value)
{

}

void UPrimitiveComponent::SetTextureParameterValue(const FString& shaderVariableName, const Texture* texture)
{

}


void UPrimitiveComponent::SetTextureParameterValue(const FString& shaderVariableName, ID3D11ShaderResourceView* texture)
{

}

void UPrimitiveComponent::SetSamplerParameterValue(const FString& shaderVariableName,  ID3D11SamplerState* sampler)
{

}


void UPrimitiveComponent::SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef /*= 0*/)
{

}


void UPrimitiveComponent::SetRasterizerState(ID3D11RasterizerState* pRasterizerState)
{

}

void UPrimitiveComponent::ImportModel(const FString& modelPath)
{
	meshPathName = modelPath;
	TArray<FString> outArrat;
	FString::Split(modelPath, FString("/"), outArrat);
	if (outArrat.size() > 0)
	{
		meshName = outArrat[outArrat.size() - 1];
	}
	else
	{
		Log_Error("ImportModel Path format Error:"+modelPath);
		meshName = modelPath;
	}
	Log_Info("ImportModel Path：" + meshPathName);
	UpdataChildMeshArrayName();
}

void UPrimitiveComponent::SetArrayMeshPrimitiveComponent(TArray<FMeshPrimitiveComponent>& arrayData)
{
	this->arrayDaya.assign(arrayData.data(), arrayData.data() + arrayData.size());
}

void UPrimitiveComponent::SetEffectShader(FString name, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs/* = nullptr*/)
{
	shaderName = name;
}

void UPrimitiveComponent::CopyPrimitiveData(UPrimitiveComponent* PrimitiveComponent)
{
	std::vector<UPrimitiveComponent::FMeshPrimitiveComponent>& xx = GetPrimitiveData();
	PrimitiveComponent->GetPrimitiveData().assign(xx.data(), GetPrimitiveData().data() + GetPrimitiveData().size());
	PrimitiveComponent->GetMeshChildArrayName().assign(GetMeshChildArrayName().data(), GetMeshChildArrayName().data() + GetMeshChildArrayName().size());
	PrimitiveComponent->SetPathName(this->GetPathName());
	PrimitiveComponent->SetMeshName(this->GetMeshName());
}

void UPrimitiveComponent::UpdataMeshPath()
{
	UPrimitiveComponent::proyxRenderData.insert(std::pair<FString, UPrimitiveComponent*>(GetMeshPathName(), this));
}

void UPrimitiveComponent::ReleaseProyx()
{
	TMap<FString, UPrimitiveComponent*>::iterator iter;

	for (iter = UPrimitiveComponent::proyxRenderData.begin(); iter != UPrimitiveComponent::proyxRenderData.end(); iter++)
	{
		if (iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
		}

	}
	//清空容器所有元素
	UPrimitiveComponent::proyxRenderData.erase(UPrimitiveComponent::proyxRenderData.begin(), UPrimitiveComponent::proyxRenderData.end());
}

std::vector<UPrimitiveComponent::FMeshPrimitiveComponent>& UPrimitiveComponent::GetPrimitiveData()
{
	return arrayDaya;
}

UPrimitiveComponent::FMeshPrimitiveComponent& UPrimitiveComponent::AddDefultPrimitiveData()
{
	arrayDaya.push_back(UPrimitiveComponent::FMeshPrimitiveComponent());
	return arrayDaya[arrayDaya.size() - 1];
}

std::vector<UPrimitiveComponent::IABufferInfo>& UPrimitiveComponent::GetPrimitiveVertexBufferData()
{
	return VertexBuffer;
}

std::vector<UPrimitiveComponent::IABufferInfo>& UPrimitiveComponent::GetPrimitiveIndexBufferData()
{
	return IndexBuffer;
}

const	UPrimitiveComponent::IABufferInfo* UPrimitiveComponent::GetVertexBufferDataFormIndex(int index)
{
	if (index>-1&& ((unsigned int)index)<GetMeshCout())
	{
		std::vector<UPrimitiveComponent::IABufferInfo>& data = GetPrimitiveVertexBufferData();
		return &data[index];
	}
	Log_Error("GetVertexBufferDataFormIndex index out of Array size");
	return nullptr;
}

const	UPrimitiveComponent::IABufferInfo* UPrimitiveComponent::GetIndexBufferDataFormIndex(int index)
{
	if (index > -1 && ((unsigned int)index) < GetMeshCout())
	{
		std::vector<UPrimitiveComponent::IABufferInfo>& data = GetPrimitiveIndexBufferData();
		return &data[index];
	}
	Log_Error("GetVertexBufferDataFormIndex index out of Array size");
	return nullptr;
}

const	UPrimitiveComponent::FMeshPrimitiveComponent* UPrimitiveComponent::GetPrimitiveDataFormIndex(int index)
{
	if (index > -1 && ((unsigned int)index) < GetMeshCout())
	{
		std::vector<UPrimitiveComponent::FMeshPrimitiveComponent>& data = GetPrimitiveData();
		return &data[index];
	}
	Log_Error("GetVertexBufferDataFormIndex index out of Array size");
	return nullptr;
}


unsigned int UPrimitiveComponent::GetMeshCout()
{
	return unsigned int(arrayDaya.size());
}

unsigned int UPrimitiveComponent::GetVertexStructSize()
{
	return sizeof(UShaderCompilerWorker::Shader_InputInfo);
}

void UPrimitiveComponent::InitBuffer()
{
		// 释放旧资源
		this->VertexBuffer.clear();
		this->IndexBuffer.clear();
		this->VertexBuffer.resize(arrayDaya.size());
		this->IndexBuffer.resize(arrayDaya.size());
		for (size_t i = 0; i < this->arrayDaya.size(); i++)
		{
			// 设置顶点缓冲区描述
			unsigned int VertexNums = GetVertexStructSize();
			D3D11_BUFFER_DESC vbd;
			ZeroMemory(&vbd, sizeof(vbd));
			vbd.Usage = D3D11_USAGE_IMMUTABLE;
			vbd.ByteWidth = (unsigned int)arrayDaya[i].vertices.size() * VertexNums;
			vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbd.CPUAccessFlags = 0;
			// 新建顶点缓冲区
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = arrayDaya[i].vertices.data();
			this->VertexBuffer[i].strides = VertexNums;
			this->VertexBuffer[i].cout = (unsigned int)arrayDaya[i].vertices.size();
			FRHI::RHI_CreateBuffer(&vbd, &InitData, &this->VertexBuffer[i].buffer);


			// 设置索引缓冲区描述
			unsigned int	IndexNums = (unsigned int)arrayDaya[i].indices.size();
			D3D11_BUFFER_DESC ibd;
			ZeroMemory(&ibd, sizeof(ibd));
			ibd.Usage = D3D11_USAGE_IMMUTABLE;
			ibd.ByteWidth = IndexNums * sizeof(unsigned int);
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibd.CPUAccessFlags = 0;
			// 新建索引缓冲区
			InitData.pSysMem = arrayDaya[i].indices.data();
			this->IndexBuffer[i].strides = sizeof(unsigned int);
			this->IndexBuffer[i].cout = (unsigned int)IndexNums;
			FRHI::RHI_CreateBuffer(&ibd, &InitData, &this->IndexBuffer[i].buffer);
			Log_Info("Load Mesh Name= " + arrayDaya[i].name + "  Vertex num=" + FString::FormatIntToString(arrayDaya[i].vertices.size()) + "  index number=" + FString::FormatIntToString(arrayDaya[i].indices.size()));
		}
}


FString UPrimitiveComponent::GetMeshPathName()
{
	return meshPathName;
}

FString UPrimitiveComponent::GetMeshName()
{
	return meshName;
}

FString UPrimitiveComponent::GetShaderName()
{
	return shaderName;
}

void UPrimitiveComponent::SetMeshPathName(const FString& pathName)
{
	this->meshPathName = pathName;
}

void UPrimitiveComponent::SetMeshName(const FString& meshName)
{
	this->meshName = meshName;
}

void UPrimitiveComponent::SetShaderName(const FString& shaderName)
{
	this->shaderName = shaderName;
}

TArray<FString>& UPrimitiveComponent::GetMeshChildArrayName()
{
	return chlidMeshName;
}


ID3D11InputLayout* UPrimitiveComponent::GetInputLayout()
{
	return g_pVertexLayout.Get();
}

ID3D11InputLayout** UPrimitiveComponent::GetInputLayoutAddress()
{
	return g_pVertexLayout.GetAddressOf();
}

void UPrimitiveComponent::UpdataChildMeshArrayName()
{
	chlidMeshName.clear();
	for (size_t i = 0; i < arrayDaya.size(); i++)
	{
		chlidMeshName.push_back(arrayDaya[i].name);
	}
}
