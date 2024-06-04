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
	virtual void OnUnPossess() override; 

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ATGCharacterPlayer> PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;

private:
	FTimerHandle DelayTimerHandle;

};
