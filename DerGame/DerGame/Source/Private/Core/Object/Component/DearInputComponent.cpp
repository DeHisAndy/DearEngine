#include "..\..\..\..\Public\Core\Object\Component\DearInputComponent.h"
#include "..\..\..\..\..\..\..\DerEngine\DerEngine\Source\Public\Core\FunctionLibrary\GamePlayStatics.h"
#include "..\..\..\..\..\..\..\DerEngine\DerEngine\Source\Public\Core\Vector\Vector.h"
#include "..\..\..\..\..\..\..\DerEngine\DerEngine\Source\Public\Core\GameMode\Pawn.h"
#include "..\..\..\..\..\..\..\DerEngine\DerEngine\Source\Public\Core\FunctionLibrary\KismetSystemLibrary.h"
#include "..\..\..\..\..\..\..\DerEngine\DerEngine\Source\Public\ThirdParty\DearImgui\imgui.h"


UDearInputComponent::UDearInputComponent()
{

}

UDearInputComponent::~UDearInputComponent()
{

}

void UDearInputComponent::BeginPlay()
{
	Super::BeginPlay();
	//×¢²áÊó±ê
	BindAxis(FInputAxisBinding(EInputEvent::IE_LeftMouse, EInputMouseEvent::IE_MouseDown, TBaseVoidDelegate(std::bind(&UDearInputComponent::MousePre, this))));
	//×¢²á¼üÅÌ
	BindAction(FInputActionBinding(EInputEvent::IE_W, EInputKeyEvent::IE_keyDown, TBaseVoidDelegate(std::bind(&UDearInputComponent::Key_W, this))));
	BindAction(FInputActionBinding(EInputEvent::IE_A, EInputKeyEvent::IE_keyDown, TBaseVoidDelegate(std::bind(&UDearInputComponent::Key_A, this))));
	BindAction(FInputActionBinding(EInputEvent::IE_S, EInputKeyEvent::IE_keyDown, TBaseVoidDelegate(std::bind(&UDearInputComponent::Key_S, this))));
	BindAction(FInputActionBinding(EInputEvent::IE_D, EInputKeyEvent::IE_keyDown, TBaseVoidDelegate(std::bind(&UDearInputComponent::Key_D, this))));
	BindAction(FInputActionBinding(EInputEvent::IE_Q, EInputKeyEvent::IE_keyDown, TBaseVoidDelegate(std::bind(&UDearInputComponent::Key_Q, this))));
	BindAction(FInputActionBinding(EInputEvent::IE_E, EInputKeyEvent::IE_keyDown, TBaseVoidDelegate(std::bind(&UDearInputComponent::Key_E, this))));
	BindAction(FInputActionBinding(EInputEvent::IE_ESC, EInputKeyEvent::IE_keyReleased, TBaseVoidDelegate(std::bind(&UDearInputComponent::Key_ESC, this))));
	UGamePlayStatics::GetPlayerPawn()->SetActorLocation(FVector(0, 0, -10));
}

void UDearInputComponent::Tick(float DeltaSeconds)
{
// 	if (ImGui::IsAnyItemActive())
// 	{
// 		SetInputMode(IE_OnlyUI);
// 	}
// 	else
// 	{
// 		SetInputMode(IE_GameAndUI);
// 	}
	Super::Tick(DeltaSeconds);
}

void UDearInputComponent::BeginDestroy()
{
	Super::BeginDestroy();
}

void UDearInputComponent::Key_W()
{
	UGamePlayStatics::GetPlayerPawn()->AddActorMovementInput(UGamePlayStatics::GetPlayerPawn()->GetActorForwardVector() , 0.05f);
}

void UDearInputComponent::Key_A()
{
	UGamePlayStatics::GetPlayerPawn()->AddActorMovementInput(UGamePlayStatics::GetPlayerPawn()->GetActorRightVector() * -1, 0.05f);
}

void UDearInputComponent::Key_S()
{
	UGamePlayStatics::GetPlayerPawn()->AddActorMovementInput(UGamePlayStatics::GetPlayerPawn()->GetActorForwardVector() * -1, 0.05f);
}

void UDearInputComponent::Key_D()
{
	UGamePlayStatics::GetPlayerPawn()->AddActorMovementInput(UGamePlayStatics::GetPlayerPawn()->GetActorRightVector(), 0.05f);
}

void UDearInputComponent::Key_Q()
{
	UGamePlayStatics::GetPlayerPawn()->AddActorMovementInput(UGamePlayStatics::GetPlayerPawn()->GetActorUpVector()*-1, 0.05f);
}

void UDearInputComponent::Key_E()
{
	UGamePlayStatics::GetPlayerPawn()->AddActorMovementInput(UGamePlayStatics::GetPlayerPawn()->GetActorUpVector(), 0.05f);
}

void UDearInputComponent::Key_ESC()
{
	UKismetSystemLibrary::WindClose();
}

void UDearInputComponent::MousePre()
{
	switch (mouseMode)
	{
	case M_UI:
		break;
	case M_3D:
		UGamePlayStatics::GetPlayerPawn()->AddActorRotation(FRotator(ImGui::GetIO().MouseDelta.x * -0.05f, 0, ImGui::GetIO().MouseDelta.y * -0.05f));
		break;
	case M_OVER:
		break;
	default:
		break;
	}

// 	UGamePlayStatics::GetPlayerPawn()->AddControllerPitchInput(io.MouseDelta.x* 0.05f);
// 	UGamePlayStatics::GetPlayerPawn()->AddControllerRollInput(io.MouseDelta.y* 0.05f);
}
