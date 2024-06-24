// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TGGameState.h"
#include "TheGate.h"

void ATGGameState::HandleBeginPlay()
{
	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ATGGameState::OnRep_ReplicatedHasBegunPlay()
{
	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("End"));
}
