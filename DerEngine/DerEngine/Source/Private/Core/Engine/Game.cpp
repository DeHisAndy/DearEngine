#include "../../../Public/Core/Engine/Game.h"
#include "../../../Public/Core/Vector/String.h"
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../Public/Core/Render/Render.h"
#include "../../../Public/Core/Object/Object/GameInstance/GameInstance.h"
#include "../../../Public/Core/Object/Object/GameInstance/GameProjectSetting.h"
#include "../../../Public/Core/Engine/GameViewport.h"



UGame* UGame::Instance = nullptr;

void UGame::RenderThread()
{

}

void UGame::Init()
{
	if (U_GameSetting)
	{
		U_GameSetting->initializationProjectSetting();
	}
	TypeAS<FRender>(F_Render, FString("GameClass Init RenderThread Fail"))->Init(); 
	if (!U_GameInstance)
	{
		Log_Info("new Defult GameInstance Class");
		U_GameInstance = TypeAS<UGameInstance>(NewObject<UGameInstance>(), FString("UGame init GameInstance Fail"));
	}
	TypeAS<UGameInstance>(U_GameInstance, FString("GameClass Init U_GameInstance=null"))->Init();
	Log_Info("GameThread Init");

}

void UGame::BeginPaly()
{
	TypeAS<FRender>(F_Render,FString("Game BeginPaly Call Render=null"))->BeginPaly();
	TypeAS<UGameInstance>(U_GameInstance, FString("Game BeginPaly Call GameInstance=null"))->BeginPaly();
	Log_Info("Game BeginPaly");
}

void UGame::OnResize()
{
	TypeAS<UGameInstance>(U_GameInstance, FString("Game OnResize Call GameInstance=null"))->OnResize();
}

void UGame::UpdateScene(float DeltaSeconds)
{
	TypeAS<UGameInstance>(U_GameInstance, FString("Game UpdateScene GameInstance=null"))->UpdateScene(DeltaSeconds);
	TypeAS<FRender>(F_Render, FString("Game UpdateScene Render=null"))->UpdateScene(DeltaSeconds);
}

void UGame::Draw(float DeltaSeconds)
{
	TypeAS<UGameInstance>(U_GameInstance, FString("Game Draw GameInstance=null"))->DrawScene(DeltaSeconds);
	TypeAS<FRender>(F_Render, FString("Game Draw Render=null"))->Draw(DeltaSeconds);
}

void UGame::Destroy()
{
	TypeAS<UGameInstance>(U_GameInstance, FString("Game Destroy GameInstance=null"))->Destroy();
	TypeAS<FRender>(F_Render, FString("Game Destroy Render=null"))->Destroy();
	Log_Info("GameThread Destroy");
}

void UGame::SetGameInstance(UGameInstance* instance)
{
	if (instance)
	{
		if (U_GameInstance)
		{
			delete U_GameInstance;
			U_GameInstance = nullptr;
		}
		U_GameInstance = instance;
	}
	else
		Log_Warning("SetGameInstance InStance=null");
}

class UGameInstance* UGame::GetGameInstance()
{
	return U_GameInstance;
}

class FRender* UGame::GetRenderThread()
{
	return F_Render;
}

class UGameProjectSetting* UGame::GetGameProjectSetting()
{
	return U_GameSetting;
}

void UGame::SetGameSetting(class UGameProjectSetting* set)
{
	if (set)
	{
		if (U_GameSetting)
		{
			delete U_GameSetting;
			U_GameSetting = nullptr;
		}
		U_GameSetting = set;
	}
	else
		Log_Warning("Game SetGameSetting==null");
}


UGame::~UGame()
{
	Log_Info("GameThread delete");
	if (F_Render)
	{
		delete F_Render;
		F_Render = nullptr;
	}
	if (U_GameInstance)
	{
		delete U_GameInstance;
		U_GameInstance = nullptr;
	}
	if (UGame::Instance)
	{
		delete UGame::Instance;
		UGame::Instance = nullptr;
	}
}

UGame* UGame::Get()
{
	if (UGame::Instance==nullptr)
	{
		UGame::Instance = new UGame();		
	}
	return UGame::Instance;
}

UGame::UGame()
{
	U_GameSetting = nullptr;
	U_GameInstance = nullptr;
	F_Render = TypeAS<FRender>(NewObject<FRender>(), FString("UGame init RenderThread Fail"));
	Log_Info(FString("Game New"));
}

