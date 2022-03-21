#include "..\..\..\..\..\Public\Core\Object\Object\Component\InputComponent.h"
#include "..\..\..\..\..\Public\Core\Helper\Log.h"
#include "..\..\..\..\..\Public\ThirdParty\DearImgui\imgui.h"

UInputComponent::UInputComponent()
{
	Log_Info("Init UInputComponent");
}

UInputComponent::~UInputComponent()
{

}

void UInputComponent::BeginPlay()
{

}

void UInputComponent::Tick(float DeltaSeconds)
{
	if (ImGui::IsAnyItemActive())
	{
		mouseMode = MouseAxisStat::M_UI;
	}
	else
	{
		mouseMode = MouseAxisStat::M_3D;
	}
	switch (inputMode)
	{
	case IE_GameAndUI:
		UpdataKeyEvent();
		UpdataMouseEvent();
		break;
	case IE_OnlyGame:
		break;
	case IE_OnlyUI:
		break;
	default:
		break;
	}

}

void UInputComponent::UpdataKeyEvent()
{
	//处理键盘事件
	for (size_t i = 0; i < bindKeyInputArray.size(); i++)
	{
		switch (bindKeyInputArray[i].style)
		{
		case IE_keyPressed:
			if (ImGui::IsKeyPressed(bindKeyInputArray[i].key))
			{
				bindKeyInputArray[i].delegate.Call();
			}
			break;
		case IE_keyReleased:
			if (ImGui::IsKeyReleased(bindKeyInputArray[i].key))
			{
				bindKeyInputArray[i].delegate.Call();
			}
			break;
		case IE_keyDown:
			if (ImGui::IsKeyDown(bindKeyInputArray[i].key))
			{
				bindKeyInputArray[i].delegate.Call();
			}
			break;
		default:
			break;
		}
	}
}

void UInputComponent::UpdataMouseEvent()
{
	//处理鼠标事件
	for (size_t i = 0; i < bindMouseInputArray.size(); i++)
	{
		switch (bindMouseInputArray[i].style)
		{
		case IE_MouseDown:
			if (ImGui::IsMouseDown(bindMouseInputArray[i].key))
			{
				bindMouseInputArray[i].delegate.Call();				
			}
			break;
		case IE_MouseReleased:
			if (ImGui::IsMouseReleased(bindMouseInputArray[i].key))
			{
				bindMouseInputArray[i].delegate.Call();
			}
			break;
		case IE_MouseClicked:
			if (ImGui::IsMouseClicked(bindMouseInputArray[i].key))
			{
				bindMouseInputArray[i].delegate.Call();
			}
			break;
		case IE_MouseDoubleClicked:
			if (ImGui::IsMouseDoubleClicked(bindMouseInputArray[i].key))
			{
				bindMouseInputArray[i].delegate.Call();
			}
			break;
		default:
			break;
		}
	}
}

void UInputComponent::BindAction(const FInputActionBinding& action)
{
	bindKeyInputArray.push_back(action);
}

void UInputComponent::BindAxis(const FInputAxisBinding& axis)
{
	bindMouseInputArray.push_back(axis);
}

void UInputComponent::SetInputMode(EGameInputMode mode)
{
	inputMode = mode;
}

EGameInputMode UInputComponent::GetInputMode()
{
	return inputMode;
}
