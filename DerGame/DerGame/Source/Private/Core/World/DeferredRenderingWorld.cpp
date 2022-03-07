#include "../../../Public/Core/World/DeferredRenderingWorld.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/FunctionLibrary/GamePlayStatics.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Render/Render.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Render/RHI.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Helper/Log.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/ThirdParty/DearImgui/imgui.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Vector/Vector2D.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Render/ShaderCompilerWorker.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Object/Object/Component/PrimitiveComponent.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Object/Actor/Actor.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/GameMode/GameMode.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Object/Object/TextureManage.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/FunctionLibrary/KismetRenderLibrary.h"
#include "../../../Public/Core/UI/DearImgui.h"
#include "../../../Core/Object/Object/Component/AssimpPrimitiveComponent.h"


UDeferredRenderingWorld::UDeferredRenderingWorld()
{
	Log_Info("new DeferredRendering Class");
}

UDeferredRenderingWorld::UDeferredRenderingWorld(AGameMode* gameMode):UWorld(gameMode)
{

}

UDeferredRenderingWorld::~UDeferredRenderingWorld()
{

}


void UDeferredRenderingWorld::Init()
{
	//��������Actor
	InitWorld();
	Super::Init();
}

void UDeferredRenderingWorld::BeginPaly()
{
	Super::BeginPaly();
}

void UDeferredRenderingWorld::UpdateScene(float DeltaSeconds)
{
	Super::UpdateScene(DeltaSeconds);

}

void UDeferredRenderingWorld::DrawScene(float DeltaSeconds)
{
	Super::DrawScene(DeltaSeconds);
}

void UDeferredRenderingWorld::Destroy()
{
	Super::Destroy();
}

void UDeferredRenderingWorld::InitWorld()
{
	//����UI
 	//UWidget::CreateWidget(new UDearImgui(), 0);
	{
		AActor* meshActor = new AActor();
		UAssimpPrimitiveComponent* assetComponent = new UAssimpPrimitiveComponent("Assets/Content/Mesh/DamagedHelmet.fbx", "Assets/Shaders/Public/VSShader.usf");
		assetComponent->SetTextureParameterValue("baseColor", Texture::CreateTextureFromFile(FString("Assets/Content/Texture/Default_albedo.jpg")));
		assetComponent->SetSamplerParameterValue("pointSample_WRAP", TextureManage::PointSamplerState_WRAP.Get());
		assetComponent->SetComponentScale3D(FVector(0.01f, 0.01f, 0.01f));
		meshActor->AddAttachComponent(assetComponent);
		this->AddActorToWorld(meshActor);
	}
	{
		AActor* meshActor = new AActor();
		UAssimpPrimitiveComponent* assetComponent = new UAssimpPrimitiveComponent("Assets/Content/Mesh/DamagedHelmet.fbx", "Assets/Shaders/Public/VSShader.usf");
		assetComponent->SetTextureParameterValue("baseColor", Texture::CreateTextureFromFile(FString("Assets/Content/loading/Loading.png")));
		assetComponent->SetSamplerParameterValue("pointSample_WRAP", TextureManage::PointSamplerState_WRAP.Get());
		assetComponent->SetComponentScale3D(FVector(0.01f, 0.01f, 0.01f));
		assetComponent->AddComponentOffset(FVector(5, 0, 0));
		meshActor->AddAttachComponent(assetComponent);
		this->AddActorToWorld(meshActor);
	}
}

