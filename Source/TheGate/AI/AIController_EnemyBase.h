// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Attack UMETA(DisplayName = "Attack"),
	Frozen UMETA(DisplayName = "Frozen"),
	Investigate UMETA(DisplayName = "Investigate"),
	Dead UMETA(DisplayName = "Dead")
};

/**
 *
 */
UCLASS()
class THEGATE_API AAIController_EnemyBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIController_EnemyBase();
	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

// States
public:
	const EAIState& GetCurrentState() const { return AIState; }
	void SetStateAsPassive();
	void SetStateAsAttack(const AActor* InTarget);
	void SetStateAsInvestigate(const FVector& InLocation);

// Perception
	// CanSenceActor는 아웃풋이 2갠데..?

	void HandleSensedSight(const FVector& InLocation);
	void HandleSensedHearing(const FVector& InLocation);
	void HandleSensedDamage(const AActor* InTarget);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	EAIState AIState;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<class ATGCharacterPlayer> PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AIPerception, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComp;

};
