// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TGCharacterStat.h"
#include "TGGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTGGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class THEGATE_API UTGGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UTGGameSingleton();
	static UTGGameSingleton& Get();
	
// Character Stat Data Section
public:
	FORCEINLINE FTGCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FTGCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FTGCharacterStat> CharacterStatTable;

};
