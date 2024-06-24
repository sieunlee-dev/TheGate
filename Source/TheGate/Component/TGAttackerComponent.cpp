// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TGAttackerComponent.h"
#include "GameData/TGEnumsDamage.h"

// Sets default values for this component's properties
UTGAttackerComponent::UTGAttackerComponent()
{
	
}


// Called when the game starts
void UTGAttackerComponent::BeginPlay()
{
	Super::BeginPlay();


}

const UTGEnumsDamage* UTGAttackerComponent::CreateEnumContainer()
{
	/*DamageEnums = NewObject<UTGEnumsDamage>();
	ensure(DamageEnums != nullptr);*/
	TObjectPtr<UTGEnumsDamage> Enums = NewObject<UTGEnumsDamage>();
	return Enums;
}

