#include "..\..\..\Public\Core\FunctionLibrary\KismetRenderLibrary.h"
#include "..\..\..\Public\Core\Render\RHI.h"
#include "..\..\..\Public\Core\Render\ShaderCompilerWorker.h"
#include "..\..\..\Public\Core\Helper\Log.h"
#include "..\..\..\Public\Core\Object\Object\Component\PrimitiveComponent.h"
#include "..\..\..\Public\Core\Object\Actor\Actor.h"
#include "..\..\..\Public\Core\Object\Object\TextureManage.h"


void UKismetRenderLibrary::RHIDX11_DrawMatrialToRenderTarget(ID3D11RenderTargetView* ppRenderTargetViews, const FString& matrial, const FString& texture2D, UPrimitiveComponent* mesh)
{
	//安全判断
	if (mesh&&CheckShaderValid(matrial))
	{
		//清理渲染画布
		FRHI::RHIDX11_ClearSwapChainRTVAndDSV(FVector4D(0.f, 0.f, 0.f, 1.f));
		FRHI::RHIDX11_SetSwapChainRenderTargetAndDepthStencilView();

		AActor* meshActor = new AActor();
		TArray<UPrimitiveComponent::FMeshPrimitiveComponent> arrayData;
		//平面创建
		UKismetRenderLibrary::CreateSquare(arrayData);
		mesh->SetArrayMeshPrimitiveComponent(arrayData);
		mesh->SetEffectShader(matrial);
		mesh->InitBuffer();
		//设置Shader参数
		mesh->SetSamplerParameterValue("pointSample", TextureManage::PointSamplerState_WRAP.Get());
		mesh->SetTextureParameterValue("renderTexture", Texture::CreateTextureFromFile(texture2D));
		meshActor->AddAttachComponent(mesh);
		//DrawCall
		meshActor->Draw(0.f);

		FRHI::RHIDX11_ExecutePresent();

 		delete meshActor;
 		meshActor = nullptr;

	}
	else
	{
		Log_Error("Not Find ShaderName=" + matrial);
	}
}

bool UKismetRenderLibrary::CheckShaderValid(const FString& matrialName)
{
	UShaderCompilerWorker* shaderMode = FRHI::RHIDX11_GetShaderCompilerWorker();
	if (shaderMode)
	{
		TArray<FString>& allShader = shaderMode->GetShaderArry();
		for (size_t i = 0; i < allShader.size(); i++)
		{
			if (allShader[i]== matrialName)
			{
				return true;
			}
		}
	}
	return false;
}

void UKismetRenderLibrary::CreateSquare(TArray<UPrimitiveComponent::FMeshPrimitiveComponent>& arrayData)
{
	arrayData.push_back(UPrimitiveComponent::FMeshPrimitiveComponent());
	arrayData[0].vertices.push_back(UShaderCompilerWorker::Shader_InputInfo(-1.0f, 1.0f, 0.1f, 1.0f, 0.0f, 0.f, 1.f,0.f,0.f));
	arrayData[0].vertices.push_back(UShaderCompilerWorker::Shader_InputInfo(1.0f, 1.0f, 0.1f, 0.0f, 1.0f, 0.f, 1.f,1.f,0.f));
	arrayData[0].vertices.push_back(UShaderCompilerWorker::Shader_InputInfo(1.0f, -1.0f, 0.1f, 0.0f, 0.0f, 1.f, 1.f,1.f,1.f));
	arrayData[0].vertices.push_back(UShaderCompilerWorker::Shader_InputInfo(-1.0f, -1.0f, 0.1f, 1.0f, 0.0f, 1.f, 1.f,0.f,1.f));
	arrayData[0].indices.push_back(0);
	arrayData[0].indices.push_back(1);
	arrayData[0].indices.push_back(2);
	arrayData[0].indices.push_back(2);
	arrayData[0].indices.push_back(3);
	arrayData[0].indices.push_back(0);
}

class AActor* UKismetRenderLibrary::CreateSquare(const FString& matrial)
{
	AActor* meshActor = new AActor();
	//平面创建
	UPrimitiveComponent* mesh = new UPrimitiveComponent();
	TArray<UPrimitiveComponent::FMeshPrimitiveComponent> arrayData;
	UKismetRenderLibrary::CreateSquare(arrayData);
	mesh->SetArrayMeshPrimitiveComponent(arrayData);
	mesh->SetEffectShader(matrial);
	mesh->InitBuffer();
	meshActor->AddAttachComponent(mesh);
	//DrawCall
	return meshActor;
}
