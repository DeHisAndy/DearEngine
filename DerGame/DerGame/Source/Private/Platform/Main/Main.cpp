#pragma once
#include <windows.h>
#include <crtdbg.h>
#include "../../../../../../DerEngine/DerEngine/Source/Public/Platform/WindowsClass.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/Vector/String.h"
#include <iostream>
#include "../../../Public/Core/Object/GameSetting.h"
#include "../../../../../../DerEngine/DerEngine/Source/Public/Core/FunctionLibrary/GamePlayStatics.h"
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPSTR cmdLine, _In_ int nCmdShow)
{

	HANDLE m_hMutex = CreateMutex(NULL, TRUE, L"StarLee");
	if ((m_hMutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
		return FALSE;
	}
	if (m_hMutex == NULL)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
	}

	CoInitialize(NULL);
	// 这些参数不使用
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	// 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//项目初始化资源设置
	UGamePlayStatics::SetGameSetting(new UGameSetting());
	WindowsClass Window(TEXT("D3D Engine Render - Editor"), hInstance, nCmdShow);
	Window.Init();
	//	WindowsClass Window(client_width, client_height, TEXT("D3D Render"), hInstance, nCmdShow);
	Window.Run();
	CoUninitialize();
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
	return 0;
}
