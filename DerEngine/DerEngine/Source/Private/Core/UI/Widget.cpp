#include "..\..\..\Public\Core\UI\Widget.h"
#include "..\..\..\Core\FunctionLibrary\GamePlayStatics.h"
#include "..\..\..\Core\Engine\GameViewport.h"
UBaseWidget::UBaseWidget()
{

}

UBaseWidget::~UBaseWidget()
{

}

UWidget::UWidget()
{

}

UWidget::~UWidget()
{

}

void UWidget::Init()
{

}

void UWidget::BeginPaly()
{

}

void UWidget::Tick(float DeltaSeconds)
{

}

void UWidget::Draw(float DeltaSeconds)
{

}

void UWidget::Destroy()
{

}

UWidget* UWidget::CreateWidget(UWidget* widget, unsigned int level)
{
	return	UGamePlayStatics::GetGameViewport()->CreateWidget(widget, level);
}
