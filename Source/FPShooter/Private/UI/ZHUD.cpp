// zoonyeok All Rights Reserved.


#include "UI/ZHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

void AZHUD::DrawHUD()
{
	Super::DrawHUD();

	//DrawCrossHair();
}

void AZHUD::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}

void AZHUD::DrawCrossHair()
{
	int32 SizeX = Canvas->SizeX; // Zero-based actual dimensions X
	int32 SizeY = Canvas->SizeY; // Zero-based actual dimensions Y
	const TInterval<float> Center(SizeX * 0.5f, SizeY * 0.5f);
	
	const float HalfLineSize = 10.f;
	const float LineThickness = 2.f;
	const FLinearColor LineColor = FLinearColor::Green;

	//수평선
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	// 수직선
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);

}
