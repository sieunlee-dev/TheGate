// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TGAIController.generated.h"

/**
 * 
 */
UCLASS()
class THEGATE_API ATGAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATGAIController();

	void RunAI();
	void StopAI();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
};
