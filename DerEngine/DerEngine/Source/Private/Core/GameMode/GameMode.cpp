#include "../../../Public/Core/GameMode/GameMode.h"
#include "../../../Public/Core/GameMode/Pawn.h"
#include "../../../Public/Core/GameMode/PlayerController.h"
#include "../../../Public/Core/Helper/Log.h"
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
AGameMode::AGameMode()
{
	this->playController = NewObject<APlayerController>();
	this->pawn = NewObject<APawn>();
	Log_Info(FString("GameMode New"));
}

AGameMode::AGameMode(class APlayerController* playController, class APawn* pawn)
{
	if (playController)
		this->playController = playController;
	else
		this->playController = NewObject<APlayerController>();

	if (pawn)
		this->pawn = pawn;
	else
		this->pawn = NewObject<APawn>();
	Log_Info(FString("GameMode New"));

}

void AGameMode::Init()
{
	TypeAS<APlayerController>(playController, FString("GameMode Init PlayerController=null"))->Init();
	TypeAS<APawn>(pawn, FString("GameMode Init Pawn=null"))->Init();
	Super::Init();
	Log_Info(FString("GameMode Init"));
}

void AGameMode::BeginPaly()
{
	TypeAS<APlayerController>(playController, FString("GameMode BeginPaly PlayerController=null"))->BeginPaly();
	TypeAS<APawn>(pawn, FString("GameMode BeginPaly Pawn=null"))->BeginPaly();
	Super::BeginPaly();
	Log_Info("GameMode BeginPaly")
}

void AGameMode::Tick(float DeltaSeconds)
{
	TypeAS<APlayerController>(playController, FString("GameMode Tick PlayerController=null"))->Tick(DeltaSeconds);
	TypeAS<APawn>(pawn, FString("GameMode Tick Pawn=null"))->Tick(DeltaSeconds);
	Super::Tick(DeltaSeconds);
}

void AGameMode::Draw(float DeltaSeconds)
{
	TypeAS<APlayerController>(playController, FString("GameMode Draw PlayerController=null"))->Draw(DeltaSeconds);
	TypeAS<APawn>(pawn, FString("GameMode Draw Pawn=null"))->Draw(DeltaSeconds);
	Super::Draw(DeltaSeconds);
}

void AGameMode::Destroy()
{
	TypeAS<APlayerController>(playController, FString("GameMode Destroy PlayerController=null"))->Destroy();
	TypeAS<APawn>(pawn, FString("GameMode Destroy Pawn=null"))->Destroy();
	Super::Destroy();
	Log_Info("GameMode Destroy");
}

void AGameMode::SetPawn(class APawn* pawn)
{
	if (pawn)
	{
		if (this->pawn)
		{
			delete this->pawn;
			this->pawn = nullptr;
		}
		this->pawn = pawn;
		return;
	}
	Log_Error("SetPawn pawn=null");
}

void AGameMode::SetPlayerController(class APlayerController* playController)
{
	if (playController)
	{
		if (this->playController)
		{
			delete this->playController;
			this->playController = nullptr;
		}
		this->playController = playController;
		return;
	}
	Log_Error("SetPlayerController playController=null");
}


class APawn* AGameMode::GetPawn()
{
	return pawn;
}

class APlayerController* AGameMode::GetPlayerController()
{
	return playController;
}

AGameMode::~AGameMode()
{
	if (playController)
	{
		delete playController;
		playController = nullptr;
	}
	if (pawn)
	{
		delete pawn;
		pawn = nullptr;
	}
	Log_Info("GameMode delete");
}

