#include "../../../Public/Core/Object/GameSetting.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/FunctionLibrary/GamePlayStatics.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Object/Object/GameInstance/GameInstance.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/GameMode/GameMode.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/ThirdParty/DearImgui/imgui.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/FunctionLibrary/Paths.h"
#include "../../../Public/Core/Object/Component/DearInputComponent.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/GameMode/DearPawn.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Render/ShaderEffect.h"
#include "../../../Public/Core/World/ForwardRenderingWorld.h"



UGameSetting::UGameSetting()
{
	SetGameFontDir("Assets/Font/");
}

UGameSetting::~UGameSetting()
{

}

void UGameSetting::initializationProjectSetting()
{
	Super::initializationProjectSetting();
	ADearPawn* pawn = new ADearPawn();
	pawn->SetupPlayerInputComponent(new UDearInputComponent());
	UGamePlayStatics::SetGameInstance(new UGameInstance(new UForwardRenderingWorld(new AGameMode(nullptr, pawn))));
}


