#include "..\..\..\Public\Core\FunctionLibrary\Paths.h"
#include "..\..\..\Public\Core\FunctionLibrary\GamePlayStatics.h"
#include "..\..\..\Public\Core\Object\Object\GameInstance\GameProjectSetting.h"
#include "..\..\..\Public\Core\FunctionLibrary\KismetSystemLibrary.h"

FString FPaths::GetGameFontDir()
{
	UGameProjectSetting* Setting = UGamePlayStatics::GetGameSetting();
	if (IsValidObject(Setting))
	{
	   return	Setting->GetGameFontDir();
	}
	return FString();
}

