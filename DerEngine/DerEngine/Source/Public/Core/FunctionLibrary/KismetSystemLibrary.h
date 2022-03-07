#pragma once
#include "../Object/Object/Object.h"
#include <windows.h>
#include "../Vector/Vector2D.h"
#include <d3d11.h>
#include "../Vector/String.h"
#include "../../../Public/Core/Helper/Log.h"
#include "../Vector/IntPoint.h"

class UKismetSystemLibrary :public UObject
{
public:
    static void ShowWindow();
    static HWND GetHWND();
    static HINSTANCE GetHINSTANCE();
    static FIntPoint GetWinSize();
    static FIntPoint GetWinPos();
    static void SetWindowSize(FIntPoint size);
    static void SetWindowPos(FIntPoint pos);
    static void SetWindowPosAndySize(FIntPoint pos,FIntPoint size);
    static FIntPoint GetDisplayScreenSize();
    static FIntPoint GetMaxScreenSize();
    static void SetWindowTitle(const FString& title);
    static int CmdShow();
    static std::wstring Gettitle();
    static float GetAspect();
    static void MessageBox_Win(std::wstring meg);
    static HWND GetWindownsHWND();
    static void HR_HRESULT(HRESULT hr);
    static bool Valid(UObject* object);
    static bool ValidAddress(const void* object);
    static void WindClose();
    static FString NewGUID();

    static FString NewGuid();
public:
    static void SetWinSize(FIntPoint size);
public:
    template<class T>
    static T* Type(void* address, FString msg = FString())
    {
		if (address == nullptr)
		{
			Log_Error(msg);
			assert(address == nullptr);
            return nullptr;
		}
        return reinterpret_cast<T*>(address);
    }
};
#define IsValidObject(object)   UKismetSystemLibrary::Valid(object)
#define HR(hr) UKismetSystemLibrary::HR_HRESULT(hr)
#define TypeAS UKismetSystemLibrary::Type
#define Check(object) UKismetSystemLibrary::ValidAddress(object)
#define checkSlow(expr) assert(expr)
