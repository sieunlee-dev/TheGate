// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TGAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TheGate.h"

UTGAnimInstance::UTGAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 10.f;
	LocomotionType = ELocomotionType::Idle;
}

void UTGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
		SprintSpeed = Movement->MaxWalkSpeed;
	}
}

void UTGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		// UCharacterMovementComponent::AddForce()
		// Velocity = PendingForceToApply * DeltaTime
#if ENABLE_DRAW_DEBUG

		//FName EnumToName = FName(TEXT("Invalid"));
		//const UEnum* Enums = FindObject<UEnum>(ANY_PACKAGE, TEXT("ELocomotionType"), true);
		//if (Enums)
		//{
		//	EnumToName = Enums->GetNameByValue((int64)LocomotionType);
		//	//EnumToName = Enums->GetNameByValue((int64)JumpmotionType);
		//}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, EnumToName.ToString());

#endif

		//bIsIdle = Velocity.IsZero();
		//bIsInAir = Velocity.Z > JumpingThreshould;

		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsInAir = Movement->IsFalling();
		bIsFlying = Movement->IsFlying();
		bIsCrouching = Movement->IsCrouching();

		CheckLocomotion();
	}
}


void UTGAnimInstance::CheckLocomotion()
{	
	switch (LocomotionType)
	{
	case ELocomotionType::Idle:
		if (!bIsIdle)
			LocomotionType = (GroundSpeed < SprintSpeed) ? ELocomotionType::Jog : ELocomotionType::Sprint;
		if (bIsInAir)
			LocomotionType = ELocomotionType::Jump;
		if (bIsCrouching)
			LocomotionType = ELocomotionType::Crouch;
		if (bIsRolling)
			LocomotionType = ELocomotionType::Roll;
		break;
	case ELocomotionType::Jog:
	case ELocomotionType::Sprint:
		if (bIsIdle)
			LocomotionType = ELocomotionType::Idle;
		if (bIsInAir)
			LocomotionType = ELocomotionType::JumpRun;
		if (bIsCrouching)
			LocomotionType = ELocomotionType::Crouch;
		if (bIsRolling)
			LocomotionType = ELocomotionType::Roll;
		break;
	case ELocomotionType::Jump:
		//if (0.1f > Super::GetRelevantAnimLength(0, 0))
		LocomotionType = ELocomotionType::JumpLoop;
		break;
	case ELocomotionType::JumpLoop:
		// 땅에 닿았고, Idle 관련 애님일 때
		if (!bIsInAir && bIsIdle)
			LocomotionType = ELocomotionType::JumpEnd;
		break;
	case ELocomotionType::JumpEnd:
		LocomotionType = ELocomotionType::Idle;
		break;
	case ELocomotionType::JumpRun:
		LocomotionType = ELocomotionType::JumpLoopRun;
		break;
	case ELocomotionType::JumpLoopRun:
		// 땅에 닿기 전, 이동 있었는지 체크
		if (!bIsInAir)
			LocomotionType = bIsIdle ? ELocomotionType::JumpEnd : ELocomotionType::JumpEndRun;
		break;
	case ELocomotionType::JumpEndRun:
		LocomotionType = ELocomotionType::Idle;
		break;
	//case ELocomotionType::JumpFly:
	//	break;
	//case ELocomotionType::Crouch:
	//	break;
	//case ELocomotionType::Roll:
	//	break;
	//case ELocomotionType::Interact:
	//	break;
	//case ELocomotionType::Interact02:
	//	break;
	//case ELocomotionType::Interact02Start:
	//	break;
	//case ELocomotionType::Interact02Loop:
	//	break;
	//case ELocomotionType::Interact02End:
	//	break;
	//case ELocomotionType::Celebrate:
	//	break;
	default:
		break;
	}
}

