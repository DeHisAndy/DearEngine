#pragma once
#include "../Object/Object/Object.h"
class UGamePlayStatics :public UObject
{
public:
	static void SetGameInstance(class UGameInstance* instance);
	static class UWorld* GetWorld();
	static class UGameInstance* GetGameInstance();
	static class APlayerController* GetPlayerController();
	static class APawn* GetPlayerPawn();
	static void SetGameSetting(class UGameProjectSetting* setting);
	static class UGameProjectSetting* GetGameSetting();
	static void SpawnActor(class AActor* actor);
	static class UGameViewport* GetGameViewport();
/*	static unsigned int AddToViewport(taskEvent task,unsigned int level=0);*/
	static void TaskToViewportLambda(taskEvent task);
};
