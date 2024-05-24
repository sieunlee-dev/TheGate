// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TGPlayerController.h"

void ATGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
