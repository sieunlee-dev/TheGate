// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TGGameMode.h"
#include "TheGate.h"
#include "TGGameState.h"

ATGGameMode::ATGGameMode()
{
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (ThirdPersonClassRef.Class)
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}	

	// DefaultPawnClass
	//static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/TheGate.TGCharacterPlayer"));  // C++ Char
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/TheGate/Player/BP_TGPlayer.BP_TGPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	
	// PlayerControllerClass = ATGPlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/TheGate.TGPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	// 게임 스테이트 C++로 지정
	// GameStateClass = ATGGameState::StaticClass();
}

#pragma region Server

/*
void ATGGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("============================================================"));
	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	// ErrorMessage = TEXT("Server is full");

	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* ATGGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("End"));

	return NewPlayerController;
}

void ATGGameMode::PostLogin(APlayerController* NewPlayer)
{
	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		for (const auto& Connection : NetDriver->ClientConnections)
		{
			TG_LOG(LogTGNetwork, Log, TEXT("Client Connections : %s"), *Connection->GetName());
		}
	}
	else
	{
		TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ATGGameMode::StartPlay()
{
	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::StartPlay();

	TG_LOG(LogTGNetwork, Log, TEXT("%s"), TEXT("End"));
}
*/

#pragma endregion

