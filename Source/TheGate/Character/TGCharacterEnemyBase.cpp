// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterEnemyBase.h"
#include "Components/SkinnedMeshComponent.h"
#include "AI/AIController_EnemyBase.h"
#include "Physics/TGCollision.h"

#include "Components/CapsuleComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATGCharacterEnemyBase::ATGCharacterEnemyBase()
{
#pragma region Set Mesh Info

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/Ent/Mesh/SK_Ent.SK_Ent"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Ent/Animations/ABP_Ent_Base.ABP_Ent_Base_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	GetMesh()->SetCollisionProfileName(CPROFILE_TGCAMERA);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_TGCAMERA);

	// 굳이 Skinned 타입 안써도 될듯...?
	SkinnedMeshComponent = Cast<USkinnedMeshComponent>(CreateDefaultSubobject<USkeletalMeshComponent>("SkinnedMesh"));
	ensure(SkinnedMeshComponent);
	// 내 스켈레탈 메시의 하위 컴포넌트로로 지정
	SkinnedMeshComponent->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterSubMeshRef(TEXT("/Game/Ent/Mesh/SK_Ent_Leaves.SK_Ent_Leaves"));
	if (CharacterSubMeshRef.Object)
	{
		SkinnedMeshComponent->SetLeaderPoseComponent(GetMesh(), true);
		SkinnedMeshComponent->SetSkeletalMesh(CharacterSubMeshRef.Object);	
		SkinnedMeshComponent->SetVisibility(IsVisability = true);
	}

	// Particle System
	// VFX
	// Hp Bar
	// Point Light

#pragma endregion

	//AIControllerClass = AAIController::StaticClass();
	AIControllerClass = AAIController_EnemyBase::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//GetMesh()->SetHiddenInGame(true);

#pragma region Montage Assets

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Game/Ent/Animations/AM_Attack_HandL.AM_Attack_HandL"));
	if (AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}

#pragma endregion

}

void ATGCharacterEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 생성자 이후에 호출하면 되는 문제였다...^^;
	AIController = Cast<AAIController_EnemyBase>(GetController());
	//AIController = Cast<AAIController>(GetController());

}

// C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\AIModule\Private\BehaviorTree\Tasks\BTTask_MoveTo.cpp
void ATGCharacterEnemyBase::MoveToTarget(const float InRadius, AActor* InGoal)
{
	FPathFollowingRequestResult ResultData;
	ResultData.Code = EPathFollowingRequestResult::RequestSuccessful;
	ResultData.Code = AIController->MoveToActor(InGoal, InRadius);

	//FAIMoveRequest MoveRequest;
	//MoveRequest.SetGoalActor(InGoal);
	//MoveRequest.SetAcceptanceRadius(InRadius);

	//FNavPathSharedPtr NavPath;
	//this->MoveTo(MoveRequest, &NavPath);

	//auto PathPoints = NavPath->GetPathPoints();
	//for (auto& Point : PathPoints)
	//{
	//	FVector Location = Point.Location;
	//	UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
	//}
}


void ATGCharacterEnemyBase::DefaultAttack(const float PlayRate)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (!AnimInstance->Montage_IsPlaying(AttackMontage))
	AnimInstance->Montage_Play(AttackMontage, PlayRate);
		
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ATGCharacterEnemyBase::DefaultAttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void ATGCharacterEnemyBase::BindAttackDelegate(const FOnAttackEndDelegate& InOnAttackEnd)
{
	OnAttackEnd = InOnAttackEnd;
}

void ATGCharacterEnemyBase::DefaultAttackEnd(UAnimMontage* InMontage, bool InbInterrupted)
{
	//Super::NotifyComboActionEnd();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//OnAttackEnd.Broadcast(true);
}
