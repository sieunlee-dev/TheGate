// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TGPlayerController.h"
#include "UI/TGHUDWidget.h"

ATGPlayerController::ATGPlayerController()
{
	//static ConstructorHelpers::FClassFinder<UTGHUDWidget> HUDWidgetRef(TEXT("/Game/TheGate/UI/WBP_PlayerHUD.WBP_PlayerHUD_C"));
	//if (HUDWidgetRef.Class)
	//{
	//	HUDWidgetClass = HUDWidgetRef.Class;
	//}
}

void ATGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// WBP 생성 (Owner, Class)
	//HUDWidget = CreateWidget<UTGHUDWidget>(this, HUDWidgetClass);
	//if (HUDWidget)
	//{
	//	HUDWidget->AddToViewport();
	//}
}
