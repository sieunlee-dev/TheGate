// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TGAttackerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGATE_API UTGAttackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTGAttackerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	const UTGEnumsDamage* CreateEnumContainer();

//	UFUNCTION(BlueprintCallable, Category = "Custom Function")
//	void MagicSpell(FTransform& OutSpawnTransform, AActor& OutTarget);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Enums")
	TObjectPtr<class UTGEnumsDamage> DamageEnums;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Enums")
	//UTGEnumsDamage* DamageEnums;

};
