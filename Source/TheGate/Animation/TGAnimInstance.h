// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TGAnimInstance.generated.h"

#define NAME(x) UMETA(DisplayName = #x)
//#define TIME_REMAINING(idx, ratio) \
//if (ratio > GetInstanceAssetPlayerTimeFromEndFraction(idx))

UENUM(BlueprintType)
enum class ELocomotionType : uint8 // 0x00~0xff
{
	Idle			= 0xff,
	Jog				= Idle ^ 1 << 0,
	Sprint			= Idle ^ 1 << 1,

	Jump			= Idle >> 1,
	JumpLoop		= Jump ^ 1 << 0,
	JumpEnd			= Jump ^ 1 << 1, // Fall
	JumpRun			= Jump ^ 1 << 2,
	JumpLoopRun		= JumpRun ^ 1 << 0,
	JumpEndRun		= JumpRun ^ 1 << 1, // Fall
	JumpFly			= Jump ^ 1 << 3,

	Crouch			= Idle >> 2,

	Roll			= Idle >> 3,

	Interact		= Idle >> 4,
	Interact02		= Interact ^ 1 << 0,
	Interact02Start	= Interact02 ^ 1 << 1,
	Interact02Loop	= Interact02 ^ 1 << 2,
	Interact02End	= Interact02 ^ 1 << 3,

	Celebrate		= Idle >> 7,
	End = 0,
};


UCLASS()
class THEGATE_API UTGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTGAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ELocomotionType LocomotionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsInAir : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFlying : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsCrouching : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRolling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float Direction;

private:
	UPROPERTY()
	float SprintSpeed;

protected:
	virtual void CheckLocomotion();
	//const float GetAnimTimeRemainingRatio(const int32& MachineIndex, const int32& StateIndex);
};
