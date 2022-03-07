#include "../../../../../Public/Core/Object/Object/GameInstance/GameProjectSetting.h"

UGameProjectSetting::UGameProjectSetting()
{

}

void UGameProjectSetting::initializationProjectSetting()
{

}

void UGameProjectSetting::SetGameFontDir(FString dir)
{
	fontPath = dir;
}

FString UGameProjectSetting::GetGameFontDir()
{
	return fontPath;
}

UGameProjectSetting::~UGameProjectSetting()
{

}

