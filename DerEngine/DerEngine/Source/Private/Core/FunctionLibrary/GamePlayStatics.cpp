#include "../../../Public/Core/Engine/Game.h"
#include "../../../Public/Core/Render/Render.h"
#include "../../../Public/Core/Object/Object/GameInstance/GameInstance.h"
#include "../../../Public/Core/FunctionLibrary/GamePlayStatics.h"
#include "../../../Public/Core/Object/Object/World/World.h"
#include "../../../Public/Core/GameMode/GameMode.h"
#include "../../../Public/Core/Object/Object/GameInstance/GameProjectSetting.h"
#include "../../../Public/Core/Helper/Log.h"
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../Core/Engine/GameViewport.h"


UGameInstance* UGamePlayStatics::GetGameInstance()
{
	return UGame::Get()->GetGameInstance();
}
void UGamePlayStatics::SetGameInstance(UGameInstance* instance)
{
	UGame::Get()->SetGameInstance(instance);
}

class UWorld* UGamePlayStatics::GetWorld()
{
return	UGamePlayStatics::GetGameInstance()->GetWorld();
}

class APlayerController* UGamePlayStatics::GetPlayerController()
{
    return	UGamePlayStatics::GetGameInstance()->GetWorld()->GetGameMode()->GetPlayerController();
}

class APawn* UGamePlayStatics::GetPlayerPawn()
{
	return UGamePlayStatics::GetGameInstance()->GetWorld()->GetGameMode()->GetPawn();
}

void UGamePlayStatics::SetGameSetting(UGameProjectSetting* setting)
{
	if (setting)
	{
		UGame::Get()->SetGameSetting(setting);
	}
	else
		Log_Warning("ProjectGameSetting Fial");
}
 UGameProjectSetting* UGamePlayStatics::GetGameSetting()
{
	 UGameProjectSetting* gameSetting = UGame::Get()->GetGameProjectSetting();
	 if (IsValidObject(gameSetting))
	 {
		 return gameSetting;
	 }
	 return nullptr;
}

void UGamePlayStatics::SpawnActor(class AActor* actor)
{
	if (IsValidObject(actor))
	{
		UGamePlayStatics::GetGameInstance()->GetWorld()->AddActorToWorld(actor);
	}
	else
	{
		Log_Error("SpawnActor actor address==null");
	}
}

UGameViewport* UGamePlayStatics::GetGameViewport()
{
	if (IsValidObject(UGamePlayStatics::GetGameInstance()))
	{
		return UGamePlayStatics::GetGameInstance()->GetGameViewport();
	}
	return nullptr;
}

// unsigned int UGamePlayStatics::AddToViewport(taskEvent task, unsigned int level/* = 0*/)
// {
// 	return	UGamePlayStatics::GetGameViewport()->AddTaskLambda(task);
// };

void UGamePlayStatics::TaskToViewportLambda(taskEvent task)
{
	UGamePlayStatics::GetGameViewport()->TaskLambda(task);
}