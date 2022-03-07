#include "..\..\..\Public\Core\Engine\GameViewport.h"
#include "..\..\..\Public\Core\Render\RHI.h"
#include "..\..\..\Public\Core\FunctionLibrary\KismetSystemLibrary.h"
#include "..\..\..\Platform\WindowsClass.h"
#include "..\..\DearImgui\imgui_impl_dx11.h"
#include "..\..\DearImgui\imgui_impl_win32.h"
bool UGameViewport::bIsGameRenderingEnabled = true;
UGameViewport::UGameViewport()
{
	Log_Info("new UGameViewport")	
	windowPos =FIntPoint();
	Log_Info("Defualt WindowPos" + windowPos.ToString())
	windowSize = FIntPoint(10,10);
	Log_Info("Defualt WindowPos" + windowSize.ToString())
	InitialPosition= FIntPoint(0, 0);
	ViewportPos = FIntPoint(0, 0);
	Log_Info("Defualt ViewportPos" + ViewportPos.ToString())
	//默认启动视口为窗口大小
	ViewportSize = windowSize;
	Log_Info("Defualt ViewportSize" + ViewportSize.ToString())
	WindowMode = EWindowMode::Windowed;
	ViewportMidMaxDepth = FVector2D(0.0f, 1.0f);
}

UGameViewport::~UGameViewport()
{
	TMulMap<unsigned int, UWidget*>::reverse_iterator  iter = widgetInstanceArray.rbegin();
	while (iter != widgetInstanceArray.rend()) {
		if (IsValidObject(iter->second))
		{
			delete iter->second;
			iter->second = nullptr;
		}
		iter++;
	}
}

void UGameViewport::Init()
{
	Log_Info("UGameViewport::Init");
	WidgetInit();
}

void UGameViewport::BeginPlay()
{
	WidgetBeginPlay();
}

void UGameViewport::Tick()
{
	WidgetTick();
}

void UGameViewport::Draw()
{
	WidgetDraw();
}

void UGameViewport::UpdataViewport(int NewPosX, int NewPosY, int NewSizeX, int NewSizeY, float MidDepth, float MaxDepth, UINT NumViewports)
{
	//保存数据
	MoveViewport(NewPosX, NewPosY, NewSizeX, NewSizeY, MidDepth, MaxDepth);
	Log_Info("Set Viewport ViewportPos=" + ViewportPos.ToString() + " ViewportSize=" + ViewportSize.ToString());
}

float UGameViewport::Aspect()
{
	return (float)ViewportSize.X / (float)ViewportSize.Y;
}

void UGameViewport::SetWindowSize(FIntPoint size)
{
	Log_Info("SetWindowSize: "+size.ToString())
	windowSize = size;
	::SetWindowPos(UKismetSystemLibrary::GetHWND(), 0, 0, 0, windowSize.X, windowSize.Y, SWP_NOMOVE);
}

FIntPoint UGameViewport::GetWindowSize()
{
	return windowSize;

}

void UGameViewport::SetWindowPos(FIntPoint pos)
{
	Log_Info("SetWindowPos: " + pos.ToString())
	windowPos = pos;
	::SetWindowPos(UKismetSystemLibrary::GetHWND(), 0, windowPos.X, windowPos.Y, 0,0, SWP_NOSIZE);
}

FIntPoint UGameViewport::GetWindowPos()
{ 
	return windowPos;

}

void UGameViewport::SetWindowPosAndySize(FIntPoint pos, FIntPoint size)
{
	Log_Info("WindowPos: " + pos.ToString()+" WindowsSize:"+size.ToString())
	windowPos = pos;
	windowSize = size;
	::SetWindowPos(UKismetSystemLibrary::GetHWND(), 0, windowPos.X, windowPos.Y, windowSize.X, windowSize.Y, SWP_SHOWWINDOW);
}

void UGameViewport::SetWindowTitle(const FString& title)
{
	this->windowtitle = title;
	::SetConsoleTitle(*this->windowtitle);
}

void UGameViewport::MoveViewport(int NewPosX, int NewPosY, int NewSizeX, int NewSizeY, float MidDepth, float MaxDepth)
{
	ViewportPos = FIntPoint(NewPosX, NewPosY);
	ViewportSize = FIntPoint(NewSizeX, NewSizeY);
	ViewportMidMaxDepth = FVector2D(MidDepth, MaxDepth);
	FRHI::RHIDX11_UpdataViewPort(ViewportPos.X, ViewportPos.Y, ViewportSize.X, ViewportSize.Y, ViewportMidMaxDepth.X, ViewportMidMaxDepth.Y);
}

void UGameViewport::Destroy()
{
	WidgetDestory();
}

void UGameViewport::OnResize()
{
	UpdataViewport(0, 0, UKismetSystemLibrary::GetWinSize().X, UKismetSystemLibrary::GetWinSize().Y);
	Log_Info("UDearImgui::Init OnResize");
}

bool UGameViewport::HasMouseCapture()
{
	return true;
}

bool UGameViewport::HasFocus() const
{
	return true;
}

bool UGameViewport::IsForegroundWindow()
{
	return true;
}

void UGameViewport::CaptureMouse(bool bCapture)
{

}

void UGameViewport::LockMouseToViewport(bool bLock)
{

}

void UGameViewport::ShowCursor(bool bVisible)
{

}

bool UGameViewport::IsCursorVisible() const
{
	return true;
}

void UGameViewport::GetMousePos(FVector2D& MousePosition, const bool bLocalPosition /*= true*/)
{

}

void UGameViewport::GetMousePos(FIntPoint& pos)
{

}

void UGameViewport::SetMouse(int x, int y)
{

}

FMatrix& UGameViewport::GetPerspectiveMatirx()
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV2, UKismetSystemLibrary::GetAspect(), 1.0f, 1000.0f);
	memcpy(&perspectiveMatirx.M, matrix.r, sizeof(__m128) * 4);
	return perspectiveMatirx;
}

void UGameViewport::WidgetInit()
{
	TMulMap<unsigned int, UWidget*>::reverse_iterator  iter = widgetInstanceArray.rbegin();
	while (iter != widgetInstanceArray.rend()) {
		if (IsValidObject(iter->second))
		{
			iter->second->Init();
		}
		iter++;
	}
}

void UGameViewport::WidgetBeginPlay()
{
	TMulMap<unsigned int, UWidget*>::reverse_iterator  iter = widgetInstanceArray.rbegin();
	while (iter != widgetInstanceArray.rend()) {
		if (IsValidObject(iter->second))
		{
			iter->second->BeginPaly();
		}
		iter++;
	}
}

void UGameViewport::WidgetTick()
{
	TMulMap<unsigned int, UWidget*>::reverse_iterator  iter = widgetInstanceArray.rbegin();
	while (iter != widgetInstanceArray.rend()) {
		if (IsValidObject(iter->second))
		{
			iter->second->Tick();
		}
		iter++;
	}
}

void UGameViewport::WidgetDraw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//执行UI类
	TMulMap<unsigned int, UWidget*>::reverse_iterator  instanceIter = widgetInstanceArray.rbegin();
	while (instanceIter != widgetInstanceArray.rend()) {
		if (IsValidObject(instanceIter->second))
		{
			instanceIter->second->Draw();
		}
		instanceIter++;
	}
	//执行UI Task任务图表
	TMap<unsigned int, taskEvent>::reverse_iterator  taskIter = taskInstanceArray.rbegin();
	while (taskIter != taskInstanceArray.rend()) {
		if (taskIter->second)
		{
			taskIter->second();
		}
		taskIter++;
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UGameViewport::WidgetDestory()
{
	TMulMap<unsigned int, UWidget*>::reverse_iterator  iter = widgetInstanceArray.rbegin();
	while (iter != widgetInstanceArray.rend()) {
		if (IsValidObject(iter->second))
		{
			iter->second->Destroy();
		}
		iter++;
	}
}

void UGameViewport::TaskLambda(taskEvent event)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	event();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

// unsigned int UGameViewport::AddTaskLambda(taskEvent event, unsigned int level/* = 0*/)
// {
// 	taskInstanceArray.insert(std::pair<unsigned int, taskEvent>(level, event));
// 	return id;
// }

UWidget* UGameViewport::CreateWidget(UWidget* widget, unsigned int level/*=0*/)
{
	if (IsValidObject(widget))
	{
		widgetInstanceArray.insert(std::pair<unsigned int, UWidget*>(level, widget));
		return widget;
	}
	else
	{
		return nullptr;
	}
}

