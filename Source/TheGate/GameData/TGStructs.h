// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TGStructs.generated.h"

/**
 * 
 */
class THEGATE_API TGStructs
{
public:
	TGStructs();
	~TGStructs();
};


USTRUCT(Atomic, BlueprintType)
struct FDamageInfo
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldDamageInvincible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeParried;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldForceInterrupt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator;
};