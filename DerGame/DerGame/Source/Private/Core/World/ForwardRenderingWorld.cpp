#include "../../../Public/Core/World/ForwardRenderingWorld.h"
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


UForwardRenderingWorld::UForwardRenderingWorld()
{
	Log_Info("new DeferredRendering Class");
}

UForwardRenderingWorld::UForwardRenderingWorld(AGameMode* gameMode) :UWorld(gameMode)
{

}

UForwardRenderingWorld::~UForwardRenderingWorld()
{

}


void UForwardRenderingWorld::Init()
{
	//创建场景Actor
	InitWorld();
	Super::Init();
}

void UForwardRenderingWorld::BeginPaly()
{
	Super::BeginPaly();
}

void UForwardRenderingWorld::UpdateScene(float DeltaSeconds)
{
	Super::UpdateScene(DeltaSeconds);

}

void UForwardRenderingWorld::DrawScene(float DeltaSeconds)
{
	Super::DrawScene(DeltaSeconds);
}

void UForwardRenderingWorld::Destroy()
{
	Super::Destroy();
}

void UForwardRenderingWorld::InitWorld()
{	 
	//绘制UI
	//UWidget::CreateWidget(new UDearImgui(), 0);
	{		
		AActor* meshActor = SpawnActor<AActor>();
		UAssimpPrimitiveComponent* assetComponent = new UAssimpPrimitiveComponent("Assets/Content/Mesh/DamagedHelmet.fbx", "Assets/Shaders/Public/VSShader.usf");
		assetComponent->SetTextureParameterValue("baseColor", Texture::CreateTextureFromFile(FString("Assets/Content/Texture/Default_albedo.jpg")));
		assetComponent->SetSamplerParameterValue("pointSample_WRAP", TextureManage::PointSamplerState_WRAP.Get());
		meshActor->AddAttachComponent(assetComponent);
		meshActor->SetActorScale3D(FVector(0.01f, 0.01f, 0.01f));
// 		AActor* meshActor = new AActor();
// 		UAssimpPrimitiveComponent* assetComponent = new UAssimpPrimitiveComponent("Assets/Content/Mesh/DamagedHelmet.fbx", "Assets/Shaders/Public/VSShader.usf");
// 		assetComponent->SetTextureParameterValue("baseColor", Texture::CreateTextureFromFile(FString("Assets/Content/Texture/Default_albedo.jpg")));
// 		assetComponent->SetSamplerParameterValue("pointSample_WRAP", TextureManage::PointSamplerState_WRAP.Get());
// 		assetComponent->SetComponentScale3D(FVector(0.01f, 0.01f, 0.01f));
// 		meshActor->AddAttchComponent(assetComponent);
// 		this->AddActorToWorld(meshActor);
	}
	// 	{
	// 		AActor* meshActor = new AActor();
	// 		UAssimpPrimitiveComponent* assetComponent = new UAssimpPrimitiveComponent("Assets/Content/Mesh/DamagedHelmet.fbx", "Assets/Shaders/Public/VSShader.usf");
	// 		assetComponent->SetTextureParameterValue("baseColor", Texture::CreateTextureFromFile(FString("Assets/Content/loading/Loading.png")));
	// 		assetComponent->SetSamplerParameterValue("pointSample_WRAP", TextureManage::PointSamplerState_WRAP.Get());
	// 		assetComponent->SetComponentScale3D(FVector(0.01f, 0.01f, 0.01f));
	// 		assetComponent->AddComponentOffset(FVector(5, 0, 0));
	// 		meshActor->AddAttchComponent(assetComponent);
	// 		this->AddActorToWorld(meshActor);
	// 	}
}

