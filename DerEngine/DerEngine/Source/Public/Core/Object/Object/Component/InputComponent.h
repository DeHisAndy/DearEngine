#pragma once
#include "ActorComponent.h"
#include "..\..\..\Delegates\Delegate.h"

enum EInputKeyEvent
{
	IE_keyPressed = 0,
	IE_keyReleased = 1,
	IE_keyDown = 2,
};
enum EInputMouseEvent
{
	IE_MouseDown = 0,
	IE_MouseReleased = 1,
	IE_MouseClicked = 2,
	IE_MouseDoubleClicked = 3,

};
enum EInputEvent
{
	IE_LeftMouse=0,
	IE_RightMouse=1,
	IE_MiddleMouse=2,
	IE_W = 87,
	IE_A = 65,
	IE_S = 83,
	IE_D = 68,
	IE_Q = 81,
	IE_E = 69,
	IE_ESC = 27,
};
enum EGameInputMode
{
	IE_GameAndUI = 0,
	IE_OnlyGame = 1,
	IE_OnlyUI = 2,
};
struct FInputActionBinding
{
	FInputActionBinding(EInputEvent _key, EInputKeyEvent _style, TBaseVoidDelegate dale)
	{
		key = _key;
		style = _style;
		delegate = dale;
	}
	~FInputActionBinding() {}
	EInputKeyEvent style;
	EInputEvent   key;
	TBaseVoidDelegate delegate;
};

struct FInputAxisBinding
{
	FInputAxisBinding(EInputEvent _key, EInputMouseEvent _style, TBaseVoidDelegate dale)
	{
		key = _key;
		style = _style;
		delegate = dale;
	}
	~FInputAxisBinding() {}
	EInputMouseEvent style;
	EInputEvent   key;
	TBaseVoidDelegate delegate;
};
enum MouseAxisStat
{
	M_UI=0,
	M_3D=1,
	M_OVER=2,
};

class UInputComponent : public UActorComponent
{
public:
	typedef UActorComponent Super;
	UInputComponent();
	virtual ~UInputComponent();
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);
	virtual void BeginDestroy() {};
public:
	void UpdataKeyEvent();
	void UpdataMouseEvent();
	void BindAction(const FInputActionBinding& action);
	void BindAxis(const FInputAxisBinding& axis);
	void SetInputMode(EGameInputMode mode);
	EGameInputMode GetInputMode();
private:
	TArray<FInputActionBinding> bindKeyInputArray;
	TArray<FInputAxisBinding> bindMouseInputArray;
protected:
	EGameInputMode inputMode;
	MouseAxisStat mouseMode;
};
