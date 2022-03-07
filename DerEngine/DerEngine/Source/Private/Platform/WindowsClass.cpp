#include <sstream>
#include <Windows.h>
#include <imm.h>
#include <string>
#include "../../Public/Platform/WindowsClass.h"
#include "../../Public/Core/Helper/Log.h"
#include "../../Public/Core/Render/Render.h"
#include "../../Public/Core/Engine/Game.h"
#include "../../Public/Core/Render/RHI.h"
#pragma comment ( lib , "imm32.lib" )

WindowsClass* WindowsClass::ClassInstance = nullptr;
LRESULT CALLBACK WindowsClass::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return	 WindowsClass::ClassInstance->MsgProc(hWnd, message, wParam, lParam);
}

WindowsClass::WindowsClass(std::wstring name, HINSTANCE hInstance, int nCmdShow) :
	m_AppPaused(false),
	m_Minimized(false),
	m_Maximized(false),
	m_Resizing(false)
{
	WinData.Instance = hInstance;
	WinData.title = name;
	WinData.CmdShow = nCmdShow;
	ClassInstance = this;


}

WindowsClass::~WindowsClass()
{
	
}

void WindowsClass::Init()
{
    Log::Get()->Initialize();
	InitMainWindow();
	InitGameThread();
}

void WindowsClass::InitGameThread()
{
	UGame::Get()->Init();
}

void WindowsClass::InitMainWindow()
{
	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	AnalysisCommandListArgs(argv, argc);
	LocalFree(argv);
	// 初始化窗口
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = WinData.Instance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"DX11 Render";
	RegisterClassEx(&windowClass);

// 	RECT windowRect = { static_cast<LONG>(WinData.WinPos.X), static_cast<LONG>(WinData.WinPos.Y), static_cast<LONG>(WinData.WinSize.X), static_cast<LONG>(WinData.WinSize.Y) };
// 	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	WinData.hwnd = CreateWindow(
		windowClass.lpszClassName,
		WinData.title.c_str(),
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
		0,
		0,
		1270,
		720,
		nullptr,        // We have no parent window.
		nullptr,        // We aren't using menus.
		WinData.Instance,
		this);
	//输入法禁用
	ImmAssociateContext(WinData.hwnd, NULL);
	Log_Info("输入法禁用")
	
	UpdateWindow(WinData.hwnd);
}

void WindowsClass::BeginPaly()
{
     	UGame::Get()->BeginPaly();
		Log_Info("Engine BeginPaly")
}

void WindowsClass::OnUpdate(float DeltaSeconds)
{
	UGame::Get()->UpdateScene(DeltaSeconds);


}
void WindowsClass::OnRender(float DeltaSeconds)
{
	UGame::Get()->Draw(DeltaSeconds);

}
void WindowsClass::OnDestroy()
{
	UGame::Get()->Destroy();
	Log_Info("Engine Destroy")
}

void WindowsClass::OnResize()
{
	UGame::Get()->OnResize();
	
}

int WindowsClass::Run()
{
	//消息循环
	MSG msg = {};
	m_Timer.Reset();
	BeginPaly();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_Timer.Tick();
			if (!m_AppPaused)
			{
				CalculateFrameStats();
				OnUpdate(m_Timer.DeltaTime());
				OnRender(m_Timer.DeltaTime());
			}
			else
			{
				Sleep(100);
			}
		}
	}
	OnDestroy();
	return static_cast<char>(msg.wParam);
}

void WindowsClass::AnalysisCommandListArgs(WCHAR* argv[], int argc)
{
	for (int i = 1; i < argc; ++i)
	{
		if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 ||
			_wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0)
		{
			WinData.title = WinData.title + L" (WARP)";
		}
	}
}

void WindowsClass::CalculateFrameStats()
{
	// 该代码计算每秒帧速，并计算每一帧渲染需要的时间，显示在窗口标题
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((m_Timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		/*PlayController::Get()->fps = fps;*/
		std::wostringstream outs;
		outs.precision(6);
		outs << "DerEngine - Editor  "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)    ";
		SetWindowText(WinData.hwnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

LRESULT WindowsClass::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!FRHI::RHIDX11_GetRenderStart())
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	if (FRHI::ImGui_ImplWin32_MsgProc(hWnd, message, wParam, lParam))
	{
		return true;
	}

	switch (message)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_AppPaused = true;
			m_Timer.Stop();
		//	Log_Info("Engine Paused")
		}
		else
		{
			m_AppPaused = false;
			m_Timer.Start();
		//	Log_Info("Engine Startd")
		}
		return 0;

		// 当用户调整窗口大小时发送WM_SIZE。 
	case WM_SIZE:
	{
		// 保存新的客户区域维度。
		//WinData.WinSize = FIntPoint(LOWORD(lParam), HIWORD(lParam));
		//Log_Info(FString("Engine OnResize Windows Size") + WinData.WinSize.ToString());
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				this->OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (m_Minimized)
				{
					m_AppPaused = false;
					m_Minimized = false;
					this->OnResize();
				}

				// Restoring from maximized state?
				else if (m_Maximized)
				{
					m_AppPaused = false;
					m_Maximized = false;
					this->OnResize();
				}
				else if (m_Resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or m_pSwapChain->SetFullscreenState.
				{
					this->OnResize();
				}
			}
		}
		return 0;

	}
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:

		m_AppPaused = true;
		m_Resizing = true;
		m_Timer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
	{
		FVector2D position = FVector2D(LOWORD(lParam), HIWORD(lParam));
		m_AppPaused = false;
		m_Resizing = false;
		m_Timer.Start();
		this->OnResize();

		return 0; 
	}
		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case VK_ESCAPE:
	{
		PostMessage(WinData.hwnd, WM_CLOSE, 0, 0);
		return 0;
	}
	// The WM_MENUCHAR message is sent when a menu is active and the user presses 
	// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

		// 监测这些键盘/鼠标事件
	case WM_INPUT:

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDOWN:

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:

	case WM_MOUSEWHEEL:
	case WM_MOUSEHOVER:
	case WM_MOUSEMOVE:
// 		if (GameThread)
// 		{
// 			PlayController::Get()->GetMouseHandle()->ProcessMessage(message, wParam, lParam);
// 			PlayController::Get()->MouseEvent();
// 		}

		return 0;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
// 		if (GameThread)
// 		{
// 			PlayController::Get()->GetKeyHandle()->ProcessMessage(message, wParam, lParam);
// 			PlayController::Get()->KeyEvent();
// 		}

		return 0;

	case WM_ACTIVATEAPP:
// 		if (GameThread)
// 		{
// 			PlayController::Get()->GetMouseHandle()->ProcessMessage(message, wParam, lParam);
// 			PlayController::Get()->GetKeyHandle()->ProcessMessage(message, wParam, lParam);
// 			PlayController::Get()->MouseEvent();
// 			PlayController::Get()->KeyEvent();
// 		}

		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

WindowsClass::WindowStruct* WindowsClass::GetWindowsStruct()
{
	return &WinData;
}

