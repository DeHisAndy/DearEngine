#include "../../../../../Public/Core/Object/Object/World/World.h"
#include "../../../../../Public/Core/Vector/String.h"
#include "../../../../../Public/Core/Helper/Log.h"
#include "../../../../../Public/Core/GameMode/GameMode.h"
#include "../../../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../../../Public/Core/Render/RHI.h"
#include "../../../../../Public/ThirdParty/DearImgui/imgui_impl_dx11.h"
#include "../../../../../Public/ThirdParty/DearImgui/imgui_impl_win32.h"

UWorld::UWorld()
{
	A_Gamemode = NewObject<AGameMode>();
	Log_Info(FString("UWorld New"));
}

UWorld::UWorld(AGameMode* GameMode)
{
	if (IsValidObject(GameMode))
	{
		SetGameMode(GameMode);
	}
}

UWorld::~UWorld()
{
	if (IsValidObject(A_Gamemode))
	{
		delete A_Gamemode;
		A_Gamemode = nullptr;
		Log_Info("World delete");
	}
	for (size_t i = 0; i < arrayActor.size(); i++)
	{
		if (IsValidObject(arrayActor[i]))
		{
			delete arrayActor[i];
			arrayActor[i] = nullptr;
		}
		
	}
}

void UWorld::Init()
{	
	TypeAS<AGameMode>(A_Gamemode, FString("UWorld Init GameMode=null"))->Init();
	for (size_t i = 0; i < arrayActor.size(); i++)
	{
		if (IsValidObject(arrayActor[i]))
		{
			arrayActor[i]->Init();
		}
	}
	Log_Info(FString("World Init"));
}

void UWorld::BeginPaly()
{
	TypeAS<AGameMode>(A_Gamemode, FString("UWorld BeginPaly GameMode=null"))->BeginPaly();
	for (size_t i = 0; i < arrayActor.size(); i++)
	{
		if (IsValidObject(arrayActor[i]))
		{
			arrayActor[i]->BeginPaly();
		}
	}
	Log_Info("World BeginPaly");
}

void UWorld::UpdateScene(float DeltaSeconds)
{
	TypeAS<AGameMode>(A_Gamemode, FString("UWorld UpdateScene GameMode=null"))->Tick(DeltaSeconds);
	for (size_t i = 0; i < arrayActor.size(); i++)
	{
		if (IsValidObject(arrayActor[i]))
		{
			arrayActor[i]->Tick(DeltaSeconds);
		}
	}
}

void UWorld::DrawScene(float DeltaSeconds)
{
	for (size_t i = 0; i < arrayActor.size(); i++)
	{
		if (IsValidObject(arrayActor[i]))
		{
			arrayActor[i]->Draw(DeltaSeconds);
		}
 	}
  	TypeAS<AGameMode>(A_Gamemode, FString("UWorld DrawScene GameMode=null"))->Draw(DeltaSeconds);
}

void UWorld::Destroy()
{
	TypeAS<AGameMode>(A_Gamemode, FString("UWorld Destroy GameMode=null"))->Destroy();
	for (size_t i = 0; i < arrayActor.size(); i++)
	{
		if (IsValidObject(arrayActor[i]))
		{
			arrayActor[i]->Destroy();
		}

	}
	Log_Info("World Destroy");
}

void UWorld::AddActorToWorld(class AActor* Actor)
{
	if (IsValidObject(Actor))
	{
		arrayActor.push_back(Actor);
	}
	else
	{
		Log_Warning("AddActorToWorld actor==null");
	}
}

AGameMode* UWorld::GetGameMode()
{
	return A_Gamemode;
}

void UWorld::SetGameMode(class AGameMode* GameMode)
{
	if (GameMode)
	{
		if (this->A_Gamemode)
		{
			delete this->A_Gamemode;
			this->A_Gamemode = nullptr;
		}
		this->A_Gamemode = GameMode;
	}
	else
		Log_Warning("world SetGameMode is null");
}

