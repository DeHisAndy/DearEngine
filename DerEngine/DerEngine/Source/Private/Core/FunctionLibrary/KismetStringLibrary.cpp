#include "../../../Public/Core/FunctionLibrary/KismetStringLibrary.h"
bool UKismetStringLibrary::GetFileSuffix(const FString& soure,  FString& suffix)
{
	suffix = "";
	TArray<FString> splitArray;
	FString::Split(soure, ".", splitArray);
	if (splitArray.size()>0)
	{
		suffix = splitArray[splitArray.size() - 1];
		if (!suffix.Empty())
		{
			return true;
		}
	}
	return false;
}
