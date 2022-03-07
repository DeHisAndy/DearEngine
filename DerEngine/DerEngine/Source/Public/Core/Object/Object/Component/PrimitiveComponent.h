#pragma once
#include "SceneComponent.h"
#include "../../../Vector/String.h"
#include "../../../Vector/Vector.h"
#include "../../../Render/ShaderCompilerWorker.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "../../../Vector/MathType.h"
class Texture;
class UPrimitiveComponent :public USceneComponent
{
public:
	struct FMeshPrimitiveComponent
	{
		FMeshPrimitiveComponent() {}
		FMeshPrimitiveComponent(
			const TArray<UShaderCompilerWorker::Shader_InputInfo>& vertices,
			const TArray<unsigned int>& indices
		)
		{
			this->vertices = vertices;
			this->indices = indices;
		}
		~FMeshPrimitiveComponent()
		{
			vertices.clear();
			indices.clear();
		}
		FString name;
		FVector MinBound;
		FVector MaxBound;
		TArray<UShaderCompilerWorker::Shader_InputInfo> vertices;
		TArray<unsigned int>        indices;
	};
	struct IABufferInfo
	{
		ComPtr<ID3D11Buffer> buffer;
		//����Ԫ���ֽڴ�С
		unsigned int strides;
		//��ǰ����
		unsigned int cout;
	};
	typedef USceneComponent Super;
	UPrimitiveComponent();
	virtual ~UPrimitiveComponent();
public:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);
	virtual void Draw();
	virtual void BeginDestroy();
	virtual void OnRegister();
	virtual void OnUnregister();
public:
	virtual	void SetVectorParameterValue(const FString& shaderVariableName,  FVector4D& vector);
	virtual	void SetBoolParameterValue(const FString& shaderVariableName, bool value);
	virtual	void SetMaterixParameterValue(const FString& shaderVariableName,  FMatrix& matrix);
	virtual	void SetScalarParameterValue(const FString& shaderVariableName, float value);
	virtual	void SetTextureParameterValue(const FString& shaderVariableName, const Texture* texture);
	virtual	void SetSamplerParameterValue(const FString& shaderVariableName,  ID3D11SamplerState* sampler);
	/*	void SetGbufferParameterValue(const FString& shaderVariableName, GbufferSRVMode mode);*/
public:
	virtual void ImportModel(const  FString& modelPath);
	virtual void SetArrayMeshPrimitiveComponent(TArray<FMeshPrimitiveComponent>& arrayData);
	virtual void SetEffectShader(FString name, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs = nullptr);
	virtual void CopyPrimitiveData(UPrimitiveComponent* PrimitiveComponent);
    //�����ص�ģ��·����ΪΨһ��ʶ�����ж��Ƿ��ظ����� ����Ѿ�����ֱ���ڴ濽������Ҫ�ٴδӱ���load
	virtual void UpdataMeshPath();
public:
	static void ReleaseProyx();
public:
	FMeshPrimitiveComponent& AddDefultPrimitiveData();
	std::vector<FMeshPrimitiveComponent>& GetPrimitiveData();
	std::vector<IABufferInfo>& GetPrimitiveVertexBufferData();
	std::vector<IABufferInfo>& GetPrimitiveIndexBufferData();
	const	IABufferInfo* GetVertexBufferDataFormIndex(int index);
	const	IABufferInfo* GetIndexBufferDataFormIndex(int index);
	const	FMeshPrimitiveComponent* GetPrimitiveDataFormIndex(int index);
	unsigned int GetMeshCout();
	unsigned int GetVertexStructSize();
	FString GetMeshPathName();
	FString GetMeshName();
	FString GetShaderName();
	void SetMeshPathName(const FString& pathName);
	void SetMeshName(const FString& meshName);
	void SetShaderName(const FString& shaderName);
	TArray<FString>& GetMeshChildArrayName();
	ID3D11InputLayout* GetInputLayout();
	ID3D11InputLayout** GetInputLayoutAddress();
	void InitBuffer();
public:
	static 	std::multimap<FString, UPrimitiveComponent*> proyxRenderData;
	std::multimap<FString, UPrimitiveComponent*>& GetProyxRenderData();
protected:
	void UpdataChildMeshArrayName();
protected:
	//ģ�ͼ�������
	TArray<FMeshPrimitiveComponent> arrayDaya;
	//ģ�Ͳ�����
	TArray<FString> chlidMeshName;
    //ģ��·��
	FString meshPathName;
	//ģ������
	FString meshName;
	//��ɫ������
	FString shaderName;
private:
	TArray<IABufferInfo>       VertexBuffer;         //���㻺����
	TArray<IABufferInfo>      IndexBuffer;           //����������
	ComPtr<ID3D11InputLayout> g_pVertexLayout;
};