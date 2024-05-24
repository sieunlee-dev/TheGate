// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/TGCharacterStatComponent.h"
#include "GameData/TGGameSingleton.h"

// Sets default values for this component's properties
UTGCharacterStatComponent::UTGCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.f;
}

// Called when the game starts
void UTGCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);	
}

void UTGCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UTGGameSingleton::Get().CharacterMaxLevel);
	BaseStat = UTGGameSingleton::Get().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f);
}

void UTGCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}

float UTGCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}