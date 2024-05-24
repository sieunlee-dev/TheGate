// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/TGItemData.h"
#include "GameData/TGCharacterStat.h"
#include "TGWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class THEGATE_API UTGWeaponItemData : public UTGItemData
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("TGItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Stat)
	FTGCharacterStat ModifierStat;

};
