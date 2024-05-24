// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/TGGameSingleton.h"

DEFINE_LOG_CATEGORY(LogTGGameSingleton);

UTGGameSingleton::UTGGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/TheGate/GameData/TGCharacterStatTable.TGCharacterStatTable"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FTGCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UTGGameSingleton& UTGGameSingleton::Get()
{
	UTGGameSingleton* Singleton = CastChecked<UTGGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogTGGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UTGGameSingleton>();
}
