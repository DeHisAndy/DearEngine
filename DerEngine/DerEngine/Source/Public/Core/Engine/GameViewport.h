#pragma once
#include "..\Object\Object\Object.h"
#include "..\Vector\IntPoint.h"
#include "..\Vector\Vector2D.h"
#include "..\UI\Widget.h"
#include "..\Vector\Matrix.h"




namespace EWindowMode
{
	enum Type
	{
		/** The window is in true fullscreen mode */
		Fullscreen,
		/** The window has no border and takes up the entire area of the screen */
		WindowedFullscreen,
		/** The window has a border and may not take up the entire screen area */
		Windowed,

		/** The total number of supported window modes */
		NumWindowModes
	};
	static inline Type ConvertIntToWindowMode(int InWindowMode)
	{
		Type WindowMode = Windowed;
		switch (InWindowMode)
		{
		case 0:
			WindowMode = Fullscreen;
			break;
		case 1:
			WindowMode = WindowedFullscreen;
			break;
		case 2:
		default:
			WindowMode = Windowed;
			break;
		}
		return WindowMode;
	}
}

class UGameViewport :public UObject
{
public:
	typedef UObject Super;
public:
	UGameViewport();
 virtual ~UGameViewport();
public:
	virtual void Init();
	virtual void BeginPlay();
	virtual void Tick();
	virtual void Draw();
	virtual void Destroy();
	virtual void OnResize();
public:
	virtual void UpdataViewport(int NewPosX, int NewPosY, int NewSizeX, int NewSizeY,float MidDepth=0.f , float MaxDepth=1.f, UINT NumViewports=1);
	virtual float Aspect();
	virtual void SetWindowSize(FIntPoint size);
	virtual void SetWindowPos(FIntPoint pos);
	virtual FIntPoint GetWindowSize();
	virtual FIntPoint GetWindowPos();
	virtual void SetWindowPosAndySize(FIntPoint pos, FIntPoint size);
	virtual void SetWindowTitle(const FString& title);
	virtual bool HasMouseCapture();
	virtual bool HasFocus() const;
	virtual bool IsForegroundWindow();
	virtual void CaptureMouse(bool bCapture);
	virtual void LockMouseToViewport(bool bLock);
	virtual void ShowCursor(bool bVisible);
	virtual bool IsCursorVisible() const;
	virtual void GetMousePos(FVector2D& MousePosition, const bool bLocalPosition = true);
	virtual void SetMouse(int x, int y);
	virtual void  GetMousePos(FIntPoint& pos);
	virtual bool IsFullscreen()	const { return WindowMode == EWindowMode::Fullscreen || WindowMode == EWindowMode::WindowedFullscreen; }
	virtual bool IsExclusiveFullscreen() const { return WindowMode == EWindowMode::Fullscreen; }
	virtual EWindowMode::Type GetWindowMode()	const { return WindowMode; }
	FIntPoint GetInitialPositionXY() const { return InitialPosition; }
	static void SetGameRenderingEnabled(bool bIsEnabled) { bIsGameRenderingEnabled = bIsEnabled; };
	static bool IsGameRenderingEnabled() { return bIsGameRenderingEnabled; }
	FMatrix& GetPerspectiveMatirx();
	FIntPoint GetViewportSize();
public:
/// <summary>
/// ui函数
/// </summary>
	virtual void WidgetInit();
	virtual void WidgetBeginPlay();
	virtual void WidgetTick();
	virtual void WidgetDraw();
	virtual void WidgetDestory();
	void TaskLambda(taskEvent event);
/*	unsigned int AddTaskLambda(taskEvent event, unsigned int level = 0);*/
	UWidget* CreateWidget(UWidget* widget,unsigned int level=0);
private:
	 void MoveViewport(int NewPosX, int NewPosY, int NewSizeX, int NewSizeY, float MidDepth, float MaxDepth);
protected:
	EWindowMode::Type WindowMode;
	//默认窗口位置
	FIntPoint InitialPosition;
	//视口大小
	FIntPoint ViewportSize;
	//视口位置
	FIntPoint ViewportPos;
	//视口深度范围
	FVector2D ViewportMidMaxDepth;
	//窗口位置
	FIntPoint windowPos;
	//窗口大小
	FIntPoint windowSize;
	//窗口标题
	FString windowtitle;

	static bool bIsGameRenderingEnabled;
	//透视矩阵
	FMatrix perspectiveMatirx;
protected:
	//ui类 key：绘制层级 数值越大越前绘制层级越高 0优先级最高最后绘制显示在最上层 map自动排序
	TMulMap<unsigned int,UWidget*>  widgetInstanceArray;
	//Task任务表
	TMulMap<unsigned int, taskEvent> taskInstanceArray;

};
