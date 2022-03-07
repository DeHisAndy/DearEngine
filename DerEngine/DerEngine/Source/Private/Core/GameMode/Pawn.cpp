#include "../../../Public/Core/GameMode/Pawn.h"
#include "../../../Public/Core/Helper/Log.h"
#include "../../../Public/Core/Object/Object/Component/InputComponent.h"
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"

void APawn::Init()
{
	Super::Init();
	Log_Info("Pawn Init")
}

void APawn::BeginPaly()
{
	Super::BeginPaly();
	Log_Info("Pawn BeginPaly")
		inputComponent->BeginPlay();
}

void APawn::Tick(float DeltaSeconds)
{
	inputComponent->Tick(DeltaSeconds);
}

void APawn::Draw(float DeltaSeconds)
{

}

void APawn::Destroy()
{
	Super::Destroy();
	inputComponent->BeginDestroy();
	Log_Info("Pawn Destroy");
}

void APawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	if (IsValidObject(InputComponent))
	{
		this->inputComponent = InputComponent;
		Log_Info("init SetupPlayerInputComponent");
	}
}

APawn::APawn()
{
	Log_Info("Pawn New");
}

APawn::~APawn()
{
	Log_Info("Pawn delete");
	if (inputComponent)
	{
		delete inputComponent;
		inputComponent = nullptr;
	}
}

void APawn::AddActorMovementInput(const FVector& direction, float magnitude)
{
	SetActorLocation(GetActorLocation() + direction.GetSafeNormal() * magnitude);
}

void APawn::AddControllerYawInput(float Val)
{
// 	FRotator ro = GetActorRotation();
// 	if (ro.Roll + Val * InputYawScale > 72|| ro.Roll + Val * InputYawScale<-72)
// 	{
// 		if (ro.Roll + Val * InputYawScale>0)
// 		{
// 			ro.Roll = 72.f;
// 			SetActorRotation(FRotator(0, 0, 72));
// 		}
// 		else
// 		{
// 			ro.Roll = -72.f;
// 			SetActorRotation(FRotator(0, 0, -72));
// 		}
// 		return;
// 	}
// 	AddActorRotationAxis(EAxis::Z, Val * InputYawScale);
	RotationInput.Yaw = Val;
}

void APawn::AddControllerPitchInput(float Val)
{
	AddActorRotation(FRotator(-Val, 0, 0));
	//AddActorRotationAxis(EAxis::Y, Val * InputPitchScale);
	//RotationInput.Pitch = Val;
}

void APawn::AddControllerRollInput(float Val)
{
	AddActorRotation(FRotator(0, 0, -Val));
	//AddActorRotationAxis(EAxis::X, Val * InputRollScale);
	//RotationInput.Roll = Val;
}



// 
// {
// 	XMFLOAT3 rotation = GetActorRotation().XMFloat3();
// 	// 将绕x轴旋转弧度限制在[-7pi/18, 7pi/18]之间
// 	rotation.x += rad;
// 	if (rotation.x > XM_PI * 7 / 18)
// 		rotation.x = XM_PI * 7 / 18;
// 	else if (rotation.x < -XM_PI * 7 / 18)
// 		rotation.x = -XM_PI * 7 / 18;
// 	FVector rotate = rotation;
// 	SetActorRotation(rotate);
// 	AddActorRotation(FRotator(ra))
// }
// 
// void APawn::Rotate(float rad)
// {
// 	XMFLOAT3 rotation = GetActorRotation().XMFloat3();
// 	rotation.y = XMScalarModAngle(rotation.y + rad);
// 	FVector rotate = rotation;
// 	SetActorRotation(rotate);
//}