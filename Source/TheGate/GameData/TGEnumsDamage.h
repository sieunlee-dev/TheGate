// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TGEnumsDamage.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	None UMETA(DisplayName = "None"),
	HitReaction UMETA(DisplayName = "HitReaction"),
	Stagger UMETA(DisplayName = "Stagger"),
	Stun UMETA(DisplayName = "Stun"),
	KnockBack UMETA(DisplayName = "KnockBack"),
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"),
	Projectile UMETA(DisplayName = "Projectile"),
	Explosion UMETA(DisplayName = "Explosion"),
	Enviroment UMETA(DisplayName = "Enviroment"),
};


UCLASS()
class THEGATE_API UTGEnumsDamage : public UObject
{
	GENERATED_BODY()
	
public:
	UTGEnumsDamage();
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Enums")
	EDamageResponse DamageResponse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Enums")
	EDamageType DamageType;
};
