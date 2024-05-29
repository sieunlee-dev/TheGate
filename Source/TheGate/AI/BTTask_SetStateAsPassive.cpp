// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetStateAsPassive.h"
#include "AI/AIController_EnemyBase.h"

UBTTask_SetStateAsPassive::UBTTask_SetStateAsPassive()
{
	NodeName = TEXT("UBTTask_SetStateAsPassive");
}

EBTNodeResult::Type UBTTask_SetStateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController_EnemyBase* AIController = Cast<AAIController_EnemyBase>(OwnerComp.GetAIOwner());
	if (nullptr == AIController)
	{
		return EBTNodeResult::Failed;
	}

	// AIController->SetState

	return EBTNodeResult::Succeeded;
}
