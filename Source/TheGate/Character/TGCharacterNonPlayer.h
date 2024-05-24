// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/TGCharacterAIInterface.h"
#include "TGCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config = TheGate)
class THEGATE_API ATGCharacterNonPlayer : public ATGCharacterBase, public ITGCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	ATGCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;
	virtual void NotifyComboActionEnd() override;
};
