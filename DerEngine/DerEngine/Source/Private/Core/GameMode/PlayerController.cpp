#include "../../../Public/Core/GameMode/PlayerController.h"
#include "../../../Public/Core/Helper/Log.h"
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
void APlayerController::Init()
{
	Super::Init();
	Log_Info("PlayerController Init")
}

void APlayerController::BeginPaly()
{
	Super::BeginPaly();
	Log_Info("PlayerController BeginPaly")
}

void APlayerController::Tick(float DeltaSeconds)
{
}

void APlayerController::Draw( )
{

}

void APlayerController::Destroy()
{
	Super::Destroy();
	Log_Info("PlayerController Destroy");
}

APlayerController::APlayerController()
{

	Log_Info("PlayerController New");
}

APlayerController::~APlayerController()
{
	Log_Info("APlayerController delete");
}

