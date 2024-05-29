// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PatrolRandomPos.h"
#include "TGAI.h"
#include "NavigationSystem.h"
#include "Interface/TGCharacterAIInterface.h"

UBTTask_PatrolRandomPos::UBTTask_PatrolRandomPos()
{
	NodeName = BTT_PATROL_RANDOMPOS;
}

EBTNodeResult::Type UBTTask_PatrolRandomPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (nullptr == AIController)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	ITGCharacterAIInterface* AIPawn = Cast<ITGCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}


	//FNavLocation NextPatrolPos;
	//float PatrolRadius = AIPawn->GetAIPatrolRadius();

	//if (NavSystem->GetRandomReachablePointInRadius(ControllingPawn->GetActorLocation(), PatrolRadius, NextPatrolPos))
	//{
	//	
	//}

	//AIController->MoveTo()

	return EBTNodeResult::Succeeded;
}
