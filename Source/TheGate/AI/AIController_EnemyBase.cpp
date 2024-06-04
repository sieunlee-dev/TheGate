// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIController_EnemyBase.h"
#include "TGAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/TGCharacterPlayer.h"
#include "Perception/AIPerceptionComponent.h"

AAIController_EnemyBase::AAIController_EnemyBase()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/TheGate/Enemies/AI/BehaviorTree/BB_Enemy_Base.BB_Enemy_Base"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/TheGate/Enemies/AI/BehaviorTree/BT_Enemy_Base.BT_Enemy_Base"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	// Perception Comp
	//AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	//AIPerceptionComponent->GetSenseConfig();

	AIState = EAIState::Passive;
}

void AAIController_EnemyBase::RunAI()
{
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerCharacter = Cast<ATGCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (nullptr == PlayerCharacter)
	{
		return;
	}

	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsObject(BBKEY_ATTACK_TARGET, PlayerCharacter);
		//SetFocus(PlayerTarget);
		//Blackboard->SetValueAsEnum(BBKEY_STATE, (uint8)AIStateType);

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AAIController_EnemyBase::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AAIController_EnemyBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// OnPossess는 BeinPlay 이전에 실행되므로
	// Set Value값들이 할당된 이후에 실행시키기 위해 딜레이를 준다
	const float Duration = .2f;
	DelayTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			RunAI();
		}), Duration, false);
}

void AAIController_EnemyBase::OnUnPossess()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
}

void AAIController_EnemyBase::SetStateAsPassive()
{
}

void AAIController_EnemyBase::SetStateAsAttack(const AActor* InTarget)
{
}

void AAIController_EnemyBase::SetStateAsInvestigate(const FVector& InLocation)
{
}

void AAIController_EnemyBase::HandleSensedSight(const FVector& InLocation)
{
}

void AAIController_EnemyBase::HandleSensedHearing(const FVector& InLocation)
{
}

void AAIController_EnemyBase::HandleSensedDamage(const AActor* InTarget)
{
}
