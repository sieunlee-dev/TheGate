// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FocusTarget.h"
#include "TGAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FocusTarget::UBTTask_FocusTarget()
{
	NodeName = BTT_FOCUS_TARGET;
}

EBTNodeResult::Type UBTTask_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (nullptr == AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ATTACK_TARGET));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	AIController->SetFocus(Target);

	return EBTNodeResult::Succeeded;
}
