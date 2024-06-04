// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ClearFocus.h"
#include "AIController.h"
#include "TGAI.h"

UBTTask_ClearFocus::UBTTask_ClearFocus()
{
	NodeName = BTT_CLEAR_FOCUS;
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (nullptr == AIController)
	{
		return EBTNodeResult::Failed;
	}

	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	
	return EBTNodeResult::Succeeded;
}
