// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TGAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UTGAnimInstance::UTGAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
	LocomotionType = ELocomotionType::Idle;
	PreLocomotionType = ELocomotionType::Idle;
	JumpmotionType = EJumpmotionType::Land;
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
		////const UEnum* Enums = FindObject<UEnum>(ANY_PACKAGE, TEXT("EJumpmotionType"), true);
		//if (Enums)
		//{
		//	EnumToName = Enums->GetNameByValue((int64)LocomotionType);
		//	//EnumToName = Enums->GetNameByValue((int64)JumpmotionType);
		//}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, EnumToName.ToString());

#endif

		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = Velocity.IsZero() /*GroundSpeed < MovingThreshould*/;
		bIsFalling = Movement->IsFalling();

#pragma region trial and error

		//if (bIsIdle)
		//{
		//	LocomotionType = ELocomotionType::Idle;
		//}
		//else
		//{
		//	if (bIsFalling)
		//	{
		//		LocomotionType = (Velocity.Z > JumpingThreshould) ? ELocomotionType::Jump : ELocomotionType::Fall;
		//	}
		//	else
		//	{
		//		LocomotionType = (GroundSpeed < SprintSpeed) ? ELocomotionType::Jog : ELocomotionType::Sprint;
		//	}
		//}

#pragma endregion

		CheckLocomotion();
		CheckJumpmotion();
		//PreLocomotionType = LocomotionType;
	}
}


void UTGAnimInstance::CheckLocomotion()
{	
	if (bIsIdle)
	{
		LocomotionType = ELocomotionType::Idle;
	}
	else
	{
		LocomotionType = (GroundSpeed < SprintSpeed) ? ELocomotionType::Jog : ELocomotionType::Sprint;
	}
}

void UTGAnimInstance::CheckJumpmotion()
{
	if (bIsFalling)
	{
		JumpmotionType = (Velocity.Z > JumpingThreshould) ? EJumpmotionType::Single : EJumpmotionType::Fall;
	}
	else
	{
		JumpmotionType = EJumpmotionType::Land;
	}
}
