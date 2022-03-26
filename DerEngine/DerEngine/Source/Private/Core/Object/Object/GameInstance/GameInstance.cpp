#include "../../../../../Public/Core/GameMode/GameMode.h"
#include "../../../../../Public/Core/Object/Object/World/World.h"
#include "../../../../../Public/Core/Helper/Log.h"
#include "../../../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../../../Public/Core/Object/Object/GameInstance/GameInstance.h"
#include "../../../../../Public/Core/Render/ShaderCompilerWorker.h"
#include "../../../../../Public/Core/Engine/GameViewport.h"
#include "../../../../../Public/Core/Object/Object/TextureManage.h"
#include "../../../../../Public/Core/FunctionLibrary/KismetRenderLibrary.h"
#include "../../../../../Public/Core/Render/RHI.h"
#include "../../../../../Public/Core/FunctionLibrary/KismetStringLibrary.h"
#include "../../../Core/Object/Object/Component/AssimpPrimitiveComponent.h"
#include "../../../Core/Render/ShaderEffect.h"
#include "../../../Core/Helper/FileHelper.h"
#include "../../DearImgui/imgui_impl_win32.h"
#include "../../DearImgui/imgui_impl_dx11.h"
#include "../../../Core/FunctionLibrary/GamePlayStatics.h"

UGameInstance::UGameInstance()
{
	this->defaultWrold = NewObject<UWorld>();
	this->defulatShaderEffect = NewObject<UShaderCompilerWorker>();
	this->defulatGameViewport = new  UGameViewport();
	Log_Info(FString("GameInstance New"));
}

UGameInstance::UGameInstance(UWorld* world, UShaderCompilerWorker* effect, UGameViewport* gameViewport)
{
	instanceLoad = false;
	if (world)
	{
		this->defaultWrold = world;
		Log_Info("Game World =" + this->defaultWrold->GetClassName());
	}
	else
	{
		this->defaultWrold = new UWorld();
		Log_Info("Game World =" + this->defaultWrold->GetClassName());
	}

	if (effect)
	{
		this->defulatShaderEffect = effect;
		Log_Info("Game ShaderEffect =" + this->defulatShaderEffect->GetClassName());
	}
	else
	{
		this->defulatShaderEffect = new UShaderEffect();
		Log_Info("Game ShaderEffect =" + this->defulatShaderEffect->GetClassName());
	}
	if (gameViewport)
	{
		this->defulatGameViewport = gameViewport;
		Log_Info("Game Viewport =" + this->defulatGameViewport->GetClassName());
	}
	else
	{
		this->defulatGameViewport = new  UGameViewport();
		Log_Info("Game Viewport =" + this->defulatGameViewport->GetClassName());
	}
}

UGameInstance::~UGameInstance()
{
	if (defaultWrold)
	{
		delete defaultWrold;
		defaultWrold = nullptr;
	}
	if (defulatShaderEffect)
	{
		delete defulatShaderEffect;
		defulatShaderEffect = nullptr;
	}
	if (defulatGameViewport)
	{
		delete defulatGameViewport;
		defulatGameViewport = nullptr;
	}
	Log_Info("GameInstance delete");
}
void TaskToViewportLambdaLogOut(FString log) {
	//渲染加载界面
	UGamePlayStatics::TaskToViewportLambda([]() {
		//设置窗口位置
		ImGui::SetNextWindowPos(ImVec2(1.f, 1.f), 0, ImVec2(0.f, 0.f));
		//设置窗口的大小
		ImGui::SetNextWindowSize(ImVec2((float)UKismetSystemLibrary::GetWinSize().X, (float)UKismetSystemLibrary::GetWinSize().Y));
		//设置窗口为透明
		ImGui::SetNextWindowBgAlpha(0);
		//设置窗口的padding为0是图片控件充满窗口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		//设置窗口为无边框
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		//创建窗口使其固定在一个位置
		ImGui::Begin("加载画面", NULL, ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoBackground);
		ImGui::Image(Texture::CreateTextureFromFile("Assets/Content/loading/Loading.png")->_srv, ImVec2((float)UKismetSystemLibrary::GetWinSize().X, (float)UKismetSystemLibrary::GetWinSize().Y));

		ImGui::End();
		ImGui::PopStyleVar(2);
		}
	);
	UGamePlayStatics::TaskToViewportLambda([=]() {
		//设置窗口位置
		ImGui::SetNextWindowPos(ImVec2(0.f, (float)UKismetSystemLibrary::GetWinSize().Y - (float)UKismetSystemLibrary::GetWinSize().Y / 14.f), 0, ImVec2(0.f, 0.f));
		//设置窗口的大小
		ImGui::SetNextWindowSize(ImVec2((float)UKismetSystemLibrary::GetWinSize().X, (float)UKismetSystemLibrary::GetWinSize().Y));
		//设置窗口的padding为0是图片控件充满窗口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		//设置窗口为无边框
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		//创建窗口使其固定在一个位置
		ImGui::Begin("加载画面", NULL, ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar);
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
		ImGui::TextWrapped(log.GetString().c_str());
		ImGui::PopStyleColor();
		ImGui::End();
		ImGui::PopStyleVar(2);
		}
	);
}

void UGameInstance::Init()
{
	//初始化Imgui
	this->ImguiInit();
	TypeAS<UGameViewport>(defulatGameViewport, FString("GameInstance Init UGameViewport=null"))->Init();
	//加载界面和初始化imgui
	this->EngineLoadingPar();
	//加载引擎所有shader文件
	TypeAS<UShaderCompilerWorker>(defulatShaderEffect, FString("GameInstance Init ShaderEffect=null"))->Init();
	//初始化引擎资源
	this->LoadEngineContentAssetsResource();
	//场景初始化
	TypeAS<UWorld>(defaultWrold, FString("GameInstance Init World=null"))->Init();
	Log_Info("GameInstance Init");
	//输出加载时所有log
	TArray<std::pair<int, FString>>& str = Log::Get()->GetData();
	for (size_t i = 0; i < str.size(); i++)
	{
		TaskToViewportLambdaLogOut(str[i].second);
	}
	//设置最终大小
	UKismetSystemLibrary::SetWindowPosAndySize(FIntPoint(0, 0), UKismetSystemLibrary::GetDisplayScreenSize());
}

void UGameInstance::BeginPaly()
{

	TypeAS<UGameViewport>(defulatGameViewport, FString("GameInstance BeginPaly UGameViewport=null"))->BeginPlay();
	TypeAS<UWorld>(defaultWrold, FString("GameInstance BeginPaly Wrold=null"))->BeginPaly();
	Log_Info("GameInstance BeginPaly")
}

void UGameInstance::UpdateScene(float DeltaSeconds)
{
	TypeAS<UGameViewport>(defulatGameViewport, FString("GameInstance Tick UGameViewport=null"))->Tick();
	TypeAS<UWorld>(defaultWrold, FString("GameInstance UpdateScene Wrold=null"))->UpdateScene(DeltaSeconds);
}

void UGameInstance::DrawScene(float DeltaSeconds)
{
	FRHI::RHI_ClearSwapChainRTVAndDSV(FVector4D(0.f, 0.f, 0.f, 1.f));
	FRHI::RHI_SetSwapChainRenderTargetAndDepthStencilView();
 	TypeAS<UWorld>(defaultWrold, FString("GameInstance DrawScene Wrold=null"))->DrawScene(DeltaSeconds);
	TypeAS<UGameViewport>(defulatGameViewport, FString("GameInstance Draw UGameViewport=null"))->Draw();
	FRHI::RHI_ExecutePresent();
}

void UGameInstance::Destroy()
{
	TypeAS<UGameViewport>(defulatGameViewport, FString("GameInstance Destroy UGameViewport=null"))->Destroy();
	TypeAS<UWorld>(defaultWrold, FString("GameInstance Destroy Wrold=null"))->Destroy();
	//删除所有纹理
	TextureManage::Get()->Destroy();
	//清空所有mesh数据
	UPrimitiveComponent::ReleaseProyx();
	Log_Info("GameInstance Destroy");

}

void UGameInstance::OnResize()
{
	GetGameViewport()->OnResize();
}

void UGameInstance::SetWorld(class UWorld* world)
{
	if (world)
	{
		if (defaultWrold)
		{
			delete this->defaultWrold;
		}
		this->defaultWrold = world;
	}
}

void UGameInstance::SetEffect(class UShaderCompilerWorker* effect)
{
	if (effect)
	{
		if (defulatShaderEffect)
		{
			delete this->defulatShaderEffect;
		}
		this->defulatShaderEffect = effect;
	}
}


void UGameInstance::ImguiInit()
{
	//初始化imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(UKismetSystemLibrary::GetWindownsHWND());
	ImGui_ImplDX11_Init(FRHI::RHI_GetD3d11Device(), FRHI::RHI_GetID3D11DeviceContext());
	io.Fonts->AddFontFromFileTTF("Assets/Font/MicrosoftYaqiHei.ttf", 16.f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;
}

void UGameInstance::EngineLoadingPar()
{
	//显示窗口
	UKismetSystemLibrary::ShowWindow();
	FIntPoint screenSize = UKismetSystemLibrary::GetDisplayScreenSize();
	UKismetSystemLibrary::SetWindowPosAndySize(screenSize/2- screenSize/6, FIntPoint(screenSize.X/3, screenSize.Y/4));

	//渲染加载界面
	UGamePlayStatics::TaskToViewportLambda([]() {
		//设置窗口位置
		ImGui::SetNextWindowPos(ImVec2(1.f, 1.f), 0, ImVec2(0.f, 0.f));
		//设置窗口的大小
		ImGui::SetNextWindowSize(ImVec2((float)UKismetSystemLibrary::GetWinSize().X, (float)UKismetSystemLibrary::GetWinSize().Y));
		//设置窗口为透明
		ImGui::SetNextWindowBgAlpha(0);
		//设置窗口的padding为0是图片控件充满窗口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		//设置窗口为无边框
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		//创建窗口使其固定在一个位置
		ImGui::Begin("加载画面", NULL, ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoBackground);
		ImGui::Image(Texture::CreateTextureFromFile("Assets/Content/loading/Loading.png")->_srv, ImVec2((float)UKismetSystemLibrary::GetWinSize().X, (float)UKismetSystemLibrary::GetWinSize().Y));

		ImGui::End();
		ImGui::PopStyleVar(2);
		}
	);

}

void UGameInstance::LoadEngineContentAssetsResource()
{
	TArray<FString> Teamp;
	FFileHelper::FindFiles("Assets/Content", ".*", Teamp);
	Log_Info("Engine Content Texture Resources Loading");
	for (int i=0;i< Teamp.size();i++)
	{
		FString suffix;
		if (UKismetStringLibrary::GetFileSuffix(Teamp[i], suffix))
		{	
			if (suffix == TEXT("png") || suffix == TEXT("jpg") || suffix == TEXT("hdr") || suffix == TEXT("HDR"))
			{
				TaskToViewportLambdaLogOut("Load... "+Teamp[i]);
				Texture::CreateTextureFromFile(Teamp[i]);
			}

		}
	}
}

class	UWorld* UGameInstance::GetWorld()
{
	return defaultWrold;
}

class	UShaderCompilerWorker* UGameInstance::GetEffect()
{
	return defulatShaderEffect;
}

class UGameViewport* UGameInstance::GetGameViewport()
{
	return defulatGameViewport;
}

