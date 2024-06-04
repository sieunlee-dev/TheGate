// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_DefaultAttack.h"
#include "TGAI.h"
#include "Interface/TGCharacterAIInterface.h"
#include "Character/TGCharacterEnemyBase.h"	// 임시 -> 인터페이스로 변경

UBTTask_DefaultAttack::UBTTask_DefaultAttack()
{
	NodeName = BTT_DEFAULT_ATTACK;
	//bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// UBTTask_Attack 클래스 참고
	ATGCharacterEnemyBase* AIPawn = Cast<ATGCharacterEnemyBase>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 1. 캐릭터에 공격 명령을 내리고 -> 추후 인터페이스로 변경
	AIPawn->DefaultAttack();

	// 2. OnAttackEnd 바인딩을 이쪽에서 진행 -> BP Assign OnAttackEnd
	FOnAttackEndDelegate OnAttackEnd;
	AIPawn->BindAttackDelegate(OnAttackEnd);

	// 3. 공격이 끝난 시점에 테스크 종료 (성공 알려주기)
	if (OnAttackEnd.IsBound())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	// 4. 공격 모션이 끝날 때까지 대기해야 하는 지연 Task
	return EBTNodeResult::InProgress;
}
