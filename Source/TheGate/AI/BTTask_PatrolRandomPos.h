// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatrolRandomPos.generated.h"

/**
 * 
 */
UCLASS()
class THEGATE_API UBTTask_PatrolRandomPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PatrolRandomPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
