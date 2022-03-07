#include "..\..\..\Public\Core\UI\DearImgui.h"
#include "..\..\..\Core\FunctionLibrary\KismetSystemLibrary.h"
#include "..\..\DearImgui\imgui.h"

UDearImgui::UDearImgui()
{

}

UDearImgui::~UDearImgui()
{

}

void UDearImgui::Init()
{
}

void UDearImgui::BeginPaly()
{
	Super::BeginPaly();
}

void UDearImgui::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void UDearImgui::Draw(float DeltaSeconds)
{

	DrawOutputLog();

}

void UDearImgui::Destroy()
{
	Super::Destroy();
}

void UDearImgui::DrawOutputLog()
{

	static size_t logNum = 0;
	if (logNum==0)
	{
		ImGui::SetNextWindowSize(ImVec2((float)UKismetSystemLibrary::GetWinSize().X, (float)UKismetSystemLibrary::GetWinSize().Y / 3.f));
		ImGui::SetNextWindowPos(ImVec2(0, UKismetSystemLibrary::GetWinSize().Y - UKismetSystemLibrary::GetWinSize().Y / 3.f));

	}
	ImGui::Begin("Output Log");
	TArray<std::pair<int,FString>>& str = Log::Get()->GetData();
// 	for (size_t i = 0; i < str.size(); i++)
// 	{
// 		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
// 		ImGui::TextWrapped(str[i].GetString().c_str());
// 		ImGui::PopStyleColor();	
// 	}
	for (size_t i = 0; i < str.size(); i++)
	{
		switch (str[i].first)
		{
		case 0:
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
			ImGui::TextWrapped(str[i].second.GetString().c_str());
			ImGui::PopStyleColor();
			break;
		case 1:
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::TextWrapped(str[i].second.GetString().c_str());
			ImGui::PopStyleColor();
			break;
		case 2:
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::TextWrapped(str[i].second.GetString().c_str());
			ImGui::PopStyleColor();
			break;
		default:
			break;
		}
	}

	if (logNum < str.size())
	{
		ImGui::SetScrollY(ImGui::GetScrollMaxY());
	}
	logNum = str.size();
	ImGui::End();


}
