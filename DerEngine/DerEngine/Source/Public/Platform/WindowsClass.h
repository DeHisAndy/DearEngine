#pragma once
#include <windows.h>
#include <string>
#include "../Core/Vector/Vector2D.h" 
#include "../Core/Helper/GameTimer.h"
#include "../Core/Vector/IntPoint.h"

class WindowsClass
{
	struct WindowStruct
	{
		HWND hwnd;
		HINSTANCE Instance;
		std::wstring title;
		int CmdShow;
		//	FVector2D Frustum;
	};
public:
	WindowsClass(std::wstring name, HINSTANCE hInstance, int nCmdShow);
	~WindowsClass();
public:
	void BeginPaly();
	void Init();
	void InitGameThread();
	void InitMainWindow();
	void OnUpdate(float DeltaSeconds);
	void OnRender(float DeltaSeconds);
	void OnDestroy();
	void OnResize();
	int Run();
	void AnalysisCommandListArgs(WCHAR* argv[], int argc);
	void CalculateFrameStats();
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	WindowStruct* GetWindowsStruct();

public:
	static	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static WindowsClass* ClassInstance;
private:
	WindowStruct WinData;
	GameTimer m_Timer;
private:
	bool      m_AppPaused;       // 应用是否暂停
	bool      m_Minimized;       // 应用是否最小化
	bool      m_Maximized;       // 应用是否最大化
	bool      m_Resizing;        // 窗口大小是否变化
};