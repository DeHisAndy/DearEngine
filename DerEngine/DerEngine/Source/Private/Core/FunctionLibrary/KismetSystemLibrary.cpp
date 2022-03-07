#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../Public/Platform/WindowsClass.h"
#include "../../../Public/Core/Object/Object/GameInstance/GameInstance.h"
#include "../../../Public/Core/Render/Render.h"
#include "../../../Public/Core/Engine/Game.h"
#include "../../../Core/FunctionLibrary/GamePlayStatics.h"
#include "../../../Core/Engine/GameViewport.h"


void UKismetSystemLibrary::ShowWindow()
{
	::ShowWindow(UKismetSystemLibrary::GetHWND(), UKismetSystemLibrary::CmdShow());
}

HWND UKismetSystemLibrary::GetHWND()
{
	return	WindowsClass::ClassInstance->GetWindowsStruct()->hwnd;
}

HINSTANCE UKismetSystemLibrary::GetHINSTANCE()
{
	return	WindowsClass::ClassInstance->GetWindowsStruct()->Instance;
}

FIntPoint UKismetSystemLibrary::GetWinSize()
{
return	UGamePlayStatics::GetGameViewport()->GetWindowSize();
}

FIntPoint UKismetSystemLibrary::GetWinPos()
{
	return	UGamePlayStatics::GetGameViewport()->GetWindowPos();
}

void UKismetSystemLibrary::SetWindowSize(FIntPoint size)
{
	UGamePlayStatics::GetGameViewport()->SetWindowSize(size);
}

void UKismetSystemLibrary::SetWindowPos(FIntPoint pos)
{
	UGamePlayStatics::GetGameViewport()->SetWindowPos(pos);
}

void UKismetSystemLibrary::SetWindowPosAndySize(FIntPoint pos, FIntPoint size)
{
	UGamePlayStatics::GetGameViewport()->SetWindowPosAndySize(pos,size);
}

FIntPoint UKismetSystemLibrary::GetDisplayScreenSize()
{
	FIntPoint size;
	RECT rtClient;
	SystemParametersInfo(SPI_GETWORKAREA, 0,&rtClient, 0);//屏幕大小(不含任务栏)
	size.X = rtClient.right;
	size.Y = rtClient.bottom;
	return size;
}

FIntPoint UKismetSystemLibrary::GetMaxScreenSize()
{
	FIntPoint size;
	size.X= GetSystemMetrics(SM_CXSCREEN);
	size.Y= GetSystemMetrics(SM_CYSCREEN);
	return size;
}

void UKismetSystemLibrary::SetWindowTitle(const FString& title)
{
	UGamePlayStatics::GetGameViewport()->SetWindowTitle(title);
}

int UKismetSystemLibrary::CmdShow()
{
	return WindowsClass::ClassInstance->GetWindowsStruct()->CmdShow;
}

std::wstring UKismetSystemLibrary::Gettitle()
{
	return	WindowsClass::ClassInstance->GetWindowsStruct()->title;
}

float UKismetSystemLibrary::GetAspect()
{
	return	UGamePlayStatics::GetGameViewport()->Aspect();
}

void UKismetSystemLibrary::SetWinSize(FIntPoint size)
{
	if (size.X>0&& size.Y>0)
	{
		UGamePlayStatics::GetGameViewport()->SetWindowSize(size);
	}
	else
		Log_Error("SetWinSize x<0&&y<0");

}

void UKismetSystemLibrary::MessageBox_Win(std::wstring meg)
{
	MessageBox(0, meg.c_str(), 0, 0);
}

HWND UKismetSystemLibrary::GetWindownsHWND()
{
	return	WindowsClass::ClassInstance->GetWindowsStruct()->hwnd;
}

void UKismetSystemLibrary::HR_HRESULT(HRESULT hr)
{
	assert(!hr);
}


bool UKismetSystemLibrary::Valid(UObject* object)
{
	if (object)
	{
		return true;
	}
	Log_Error("addres null");
	return false;
}

bool UKismetSystemLibrary::ValidAddress(const void* object)
{
	if (object)
	{
		return true;
	}
	Log_Error("addres null");
	return false;
}

void UKismetSystemLibrary::WindClose()
{
	PostMessage(UKismetSystemLibrary::GetHWND(), WM_CLOSE, 0, 0);
}

FString UKismetSystemLibrary::NewGUID()
{
	GUID guid;
	CoCreateGuid(&guid);
	char cBuffer[64] = { 0 };
	sprintf_s(cBuffer, sizeof(cBuffer),
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0],
		guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4],
		guid.Data4[5], guid.Data4[6],
		guid.Data4[7]);
	return FString(cBuffer);
}
